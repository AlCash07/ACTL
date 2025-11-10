// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_AND_INITIALIZE.hpp>
#include <actl/functional/parameter/out.hpp>

namespace ac {

namespace detail {
template<typename T>
struct is_inout : std::false_type {};
} // namespace detail

/// Checks whether T is a (possibly cvref-qualified) ac::inout wrapper.
/// @note If the program adds specializations for ac::is_inout_v,
/// the behavior is undefined.
template<typename T>
inline constexpr bool is_inout_v =
    detail::is_inout<std::remove_cvref_t<T>>::value;

/// Thin wrapper over a Reference type
/// to specify an input-output function parameter.
///
/// Improves code clarity by making variable modifications clearly visible
/// at both the function definition and all the call sites.
/// For example, in operations similar to sort it's not immediately clear
/// if we modify the argument in-place or return a new value:
/// @code
/// sort_range(x);
/// @endcode
/// Using ac::inout wrapper, we can provide two obviously differentiated
/// overloads, with `[[nodiscard]]` as another measure to prevent incorrect use:
/// @code
/// template<typename Range>
/// void sort_range(ac::inout<Range&> range) { ... }
///
/// template<typename Range>
/// [[nodiscard]] Range sort_range(Range const& range) { ... }
///
/// sort_range(ac::inout{x});
/// auto y = sort_range(x);
/// @endcode
template<Reference Ref>
class inout : public out<Ref> {
public:
    /// Constructor from an arbitrary type convertible to the wrapped type.
    template<typename Arg>
        requires(std::is_constructible_v<Ref, Arg &&> && !is_out_v<Arg>)
    explicit constexpr inout(Arg&& arg)
        AC_DEDUCE_NOEXCEPT_AND_INITIALIZE(out<Ref>{std::forward<Arg>(arg)}) {}

    /// See analogous ac::out constructor.
    template<typename Source>
        requires(std::is_convertible_v<Source, Ref>)
    constexpr inout(inout<Source>&& source)
        AC_DEDUCE_NOEXCEPT_AND_INITIALIZE(out<Ref>{*source}) {}

    /// Analogous to `ac::out<Ref>::operator=`.
    /// Overridden to change the return type from ac::out& to ac::inout&.
    template<typename Source>
    constexpr inout& operator=(Source&& source)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN( //
            this->out<Ref>::operator=(std::forward<Source>(source)),
            *this
        )
};

template<Reference Ref>
inout(Ref&&) -> inout<Ref>;

namespace detail {
template<Reference Ref>
struct is_inout<inout<Ref>> : std::true_type {};
template<Reference Ref>
struct is_out<inout<Ref>> : std::true_type {};
} // namespace detail

} // namespace ac
