// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/container/array/static_array.hpp>
#include <actl/container/conversion/convert_to.hpp>
#include <actl/container/equal_sequences/arrays.hpp>
#include <actl/container/extent.hpp>
#include <actl/functional/deduce_noexcept.hpp>

namespace ac {

/// @class semi_static_array is an array with some of the elements known at
/// compile-time. Specifically, for each element of @p Values:
/// - if it's equal to `dynamic_extent<T>`, then the actual value is specified
/// at runtime;
/// - otherwise, the value is fixed at compile-time.
template <class T, T... Values>
class semi_static_array
{
    static constexpr size_t size_dyn_v =
        (0 + ... + size_t{Values == dynamic_extent<T>});

public:
    using value_type = T;
    using size_type = size_t;

    static constexpr size_constant<sizeof...(Values)> size() noexcept
    {
        return {};
    }

    static constexpr size_constant<size_dyn_v> size_dynamic() noexcept
    {
        return {};
    }

    static_assert(size_dyn_v > 0);

    static constexpr auto static_values = static_array<T, Values...>{};

    std::array<T, size_dyn_v> dynamic_values;

    constexpr semi_static_array() noexcept = default;

    constexpr semi_static_array(extent_holder_t<T, Values>... xs) noexcept
        : semi_static_array{indices, xs...}
    {}

    // The first parameter is needed because of the bug in std::is_trivial impl.
    template <class T0, class... Ts>
        requires(
            1 + sizeof...(Ts) == size_dyn_v && std::is_convertible_v<T0, T> &&
            (... && std::is_convertible_v<Ts, T>))
    constexpr semi_static_array(T0 x0, Ts... xs) noexcept
        : dynamic_values{static_cast<T>(x0), static_cast<T>(xs)...}
    {}

    constexpr T operator[](size_t i) const noexcept(AC_ASSERT_IS_NOEXCEPT())
    {
        AC_ASSERT(i < size());
        if (static_values[i] != dynamic_extent<T>)
            return static_values[i];
        else
            return dynamic_values[dynamic_index_f(i)];
    }

    /// Indexing by a compile-time constant results in a compile-time constant
    /// whenever possible.
    template <auto I>
    constexpr auto operator[](constant<I> i) const noexcept
    {
        if constexpr (static_values[i] != dynamic_extent<T>)
            return static_values[i];
        else
            return dynamic_values[dynamic_index<I>];
    }

    template <auto I>
    constexpr auto operator[](constant<I>) noexcept
        -> std::enable_if_t<static_values[I] == dynamic_extent<T>, T&>
    {
        return dynamic_values[dynamic_index<I>];
    }

    // Structured binding support.
    template <size_t I>
    friend constexpr auto get(semi_static_array const& src) noexcept
    {
        return src[size_constant<I>{}];
    }

    friend constexpr void swap(
        semi_static_array& lhs, semi_static_array& rhs) noexcept
    {
        lhs.dynamic_values.swap(rhs.dynamic_values);
    }

    friend constexpr auto operator==(
        semi_static_array const& lhs, semi_static_array const& rhs) noexcept
    {
        return equal_arrays(lhs.dynamic_values, rhs.dynamic_values);
    }

private:
    static constexpr auto indices = std::make_index_sequence<size()>{};

    template <size_t... Is>
    static constexpr size_t dymanic_index_impl(
        std::index_sequence<Is...>) noexcept
    {
        return (0 + ... + size_t{static_values[Is] == dynamic_extent<T>});
    }

    template <size_t I>
    static constexpr size_t dynamic_index =
        dymanic_index_impl(std::make_index_sequence<I>{});

    static constexpr size_t dynamic_index_f(size_t i)
    {
        size_t result = 0;
        for (size_t j = 0; j < i; ++j)
            result += static_values[j] == dynamic_extent<T>;
        return result;
    }

    template <size_t I>
    constexpr void assign_at([[maybe_unused]] T x) noexcept
    {
        if constexpr (static_values[I] == dynamic_extent<T>)
            dynamic_values[dynamic_index<I>] = x;
    }

    template <size_t... Is>
    explicit constexpr semi_static_array(
        std::index_sequence<Is...>, extent_holder_t<T, Values>... xs) noexcept
        : dynamic_values{}
    {
        (..., assign_at<Is>(xs));
    }
};

template <class... Ts>
    requires are_same_v<unwrap_constant_t<Ts>...>
semi_static_array(Ts...) -> semi_static_array<
    std::common_type_t<unwrap_constant_t<Ts>...>,
    static_extent_v<Ts>...>;

template <class T, T... Values, class... Args>
    requires(
        sizeof...(Args) == semi_static_array<T, Values...>::size_dynamic() &&
        (... && can_convert_to_v<T, Args>))
struct conversion<semi_static_array<T, Values...>, Args...> : std::true_type
{
    static constexpr auto convert(Args&&... args)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(semi_static_array<T, Values...>{
            convert_to<T>(std::forward<Args>(args))...})
};

} // namespace ac

namespace std {

template <class T, T... Values>
struct tuple_size<ac::semi_static_array<T, Values...>>
{
    static constexpr size_t value = sizeof...(Values);
};

template <size_t I, class T, T... Values>
struct tuple_element<I, ac::semi_static_array<T, Values...>>
{
    using type = decltype(get<I>(ac::semi_static_array<T, Values...>{}));
};

} // namespace std
