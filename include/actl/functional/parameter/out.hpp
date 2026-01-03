// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN.hpp>
#include <actl/meta/Reference.hpp>
#include <memory> // for std::addressof
#include <type_traits>
#include <utility> // for std::forward

namespace ac {

namespace detail {
template<typename T>
struct is_out : std::false_type {};
} // namespace detail

/// Checks whether T is a (possibly cvref-qualified) ac::out wrapper.
///
/// @note If the program adds specializations for ac::is_inout_v,
/// the behavior is undefined.
template<typename T>
inline constexpr bool is_out_v = detail::is_out<std::remove_cvref_t<T>>::value;

/// Thin wrapper over a Reference type
/// to specify an output-only function parameter.
///
/// Improves code clarity by making variable modifications clearly visible
/// at both the function definition and all the call sites.
/// For example, in operations similar to copy it's not immediately clear
/// which of the arguments we're copying to:
/// @code
/// copy_range(x, y);
/// @endcode
/// Using ac::out wrapper, we can make it obvious
/// without any extra documentation:
/// @code
/// template<typename Range>
/// void copy_range(out<Range&> target, Range const& source) { ... }
///
/// copy_range(out{x}, y);
/// @endcode
template<Reference Ref>
class out {
public:
    /// Constructor from an arbitrary type convertible to the wrapped type.
    template<typename Arg>
        requires(std::is_constructible_v<Ref, Arg &&> && !is_out_v<Arg>)
    explicit constexpr out(Arg&& arg) //
        noexcept(std::is_nothrow_constructible_v<Ref, Arg&&>)
        : m_ref{std::forward<Arg>(arg)} {}

    /// Function arguments in C++ can undergo an implicit conversion
    /// to match the parameter types.
    /// For example, `std::vector<int>&` to `std::span<int>`.
    ///
    /// This implicit constructor allows to preserve this behavior
    /// without the extra manual conversion when ac::out wrapper is used
    /// if the wrapped types are implicitly convertible.
    /// For example, `ac::out<std::vector<int>&>` argument can be passed
    /// where `ac::out<std::span<int>>` parameter is expected.
    template<typename Source>
        requires(std::is_convertible_v<Source, Ref>)
    constexpr out(out<Source>&& source) //
        noexcept(std::is_nothrow_constructible_v<Ref, Source>)
        : m_ref{*source} {}

    /// Accesses the wrapped value.
    ///
    /// @note operator* is used for consistency with `std::optional`.
    /// Simple operator like this is even more appropriate here than in
    /// `std::optional`, because `std::optional` might not contain a value,
    /// but here it's always available.
    constexpr Ref operator*() const noexcept {
        return m_ref;
    }

    /// Implicit conversion to the wrapped type.
    constexpr operator Ref() const noexcept {
        return **this;
    }

    /// Provides direct access to the members of the wrapped type
    /// as `out->member`.
    ///
    /// @note It would be better to support `out.member` syntax,
    /// but C++ doesn't allow to overload `operator.` yet.
    constexpr std::remove_reference_t<Ref>* operator->() noexcept {
        return std::addressof(m_ref);
    }

    /// operator= enables assignment extension by overloading
    /// the `assign` free function accessible via ADL.
    ///
    /// For example, if a third-party `Container` doesn't support
    /// assignment from a range like
    /// @code
    /// targetContainer = sourceRange;
    /// @endcode
    /// we can still achieve a similar syntax
    /// @code
    /// ac::out{targetContainer} = sourceRange;
    /// @endcode
    /// by implementing the `assign` function
    /// in the same namespace as `Container`:
    /// @code
    /// template<typename SourceRange>
    /// void assign(ac::out<Container&> target, SourceRange const& source) {
    ///     ...
    /// }
    /// @endcode
    ///
    /// @return The wrapped reference, which is inconsistent
    /// with the convention to return `*this` from operator=.
    /// However, it's intended to not return ac::out here,
    /// because each usage of a reference as an output parameter
    /// should be separately wrapped into ac::out.
    template<typename Source>
    constexpr Ref operator=(Source&& source)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(
            assign(*this, std::forward<Source>(source))
        )

    struct enable_operators;

private:
    Ref m_ref;
};

template<Reference Ref>
out(Ref&&) -> out<Ref>;

namespace detail {
template<Reference Ref>
struct is_out<out<Ref>> : std::true_type {};
} // namespace detail

template<typename Target, typename Source>
constexpr Target assign(out<Target>& target, Source&& source)
    AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN( //
            * target = std::forward<Source>(source)
    )

} // namespace ac
