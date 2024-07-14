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
template<class T>
struct is_inout : std::false_type {};
} // namespace detail

/// Checks whether T is a (possibly cvref-qualified) ac::inout wrapper.
/// @note If the program adds specializations for ac::is_inout_v,
/// the behavior is undefined.
template<class T>
inline constexpr bool is_inout_v =
    detail::is_inout<std::remove_cvref_t<T>>::value;

/// Thin wrapper over a reference-like or a pointer-like type
/// to specify an input-output function parameter.
///
/// It helps to simplify reasoning about the code behavior
/// by making potential variable modifications clearly visible
/// at both the function definition and all the call sites.
///
/// For example, in operations similar to sort it's not immediately clear
/// if we modify the argument in-place or return a new value:
/// ```
/// sort_range(x);
/// ```
/// Using ac::inout wrapper, we can provide two clearly differentiated
/// overloads, with `[[nodiscard]]` as another measure to prevent incorrect use:
/// ```
/// template<class Range>
/// void sort_range(ac::inout<Range&> range) { ... }
///
/// template<class Range>
/// [[nodiscard]] Range sort_range(Range const& range) { ... }
///
/// sort_range(ac::inout{x});
/// auto y = sort_range(x);
/// ```
template<class T>
class inout : public out<T> {
public:
    /// Constructor from an arbitrary type convertible to the wrapped type.
    template<class Arg>
        requires(std::is_constructible_v<T, Arg &&> && !is_out_v<Arg>)
    explicit constexpr inout(Arg&& arg)
        AC_DEDUCE_NOEXCEPT_AND_INITIALIZE(out<T>{std::forward<Arg>(arg)}) {}

    /// See analogous ac::out constructor.
    template<class U>
        requires(std::is_convertible_v<U, T>)
    constexpr inout(inout<U>&& src)
        AC_DEDUCE_NOEXCEPT_AND_INITIALIZE(out<T>{*src}) {}

    /// Analogous to `ac::out<T>::operator=`.
    /// Overridden to change the return type from ac::out& to ac::inout&.
    template<class Src>
    constexpr inout& operator=(Src&& src)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(
            out<T>::operator=(std::forward<Src>(src)), *this
        )
};

template<class T>
inout(T&&) -> inout<T>;

namespace detail {
template<class T>
struct is_inout<inout<T>> : std::true_type {};
template<class T>
struct is_out<inout<T>> : std::true_type {};
} // namespace detail

} // namespace ac
