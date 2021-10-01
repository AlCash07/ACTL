// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/container/array/static_array.hpp>
#include <actl/container/extent.hpp>

namespace ac {

namespace detail {

// TODO: remove when std::array operator== is constexpr.
template <class Array>
constexpr bool equal_array(const Array& lhs, const Array& rhs) noexcept
{
    for (size_t i = 0; i != lhs.size(); ++i)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}

} // namespace detail

template <class T, T... Values>
class semi_static_array
{
public:
    using value_type = T;
    using size_type = size_t;

    static constexpr size_t size() noexcept
    {
        return sizeof...(Values);
    }

    static constexpr size_t size_dynamic() noexcept
    {
        return (0 + ... + size_t{Values == dynamic_extent<T>});
    }

    static_assert(size_dynamic() > 0);

    static constexpr auto static_values = static_array<T, Values...>{};

    std::array<T, size_dynamic()> dynamic_values;

    constexpr semi_static_array() noexcept = default;

    explicit constexpr semi_static_array(
        extent_holder_t<T, Values>... xs) noexcept
        : semi_static_array{indexes, xs...}
    {}

    // The first parameter is needed because of the bug in std::is_trivial impl.
    template <
        class T0,
        class... Ts,
        enable_int_if<
            sizeof...(Ts) + 1 == size_dynamic() &&
            std::is_convertible_v<T0, T> &&
            (... && std::is_convertible_v<Ts, T>)> = 0>
    explicit constexpr semi_static_array(T0 x0, Ts... xs) noexcept
        : dynamic_values{static_cast<T>(x0), static_cast<T>(xs)...}
    {}

    constexpr T operator[](size_t i) const noexcept(ACTL_ASSERT_IS_NOEXCEPT())
    {
        ACTL_ASSERT(i < size());
        if (static_values[i] != dynamic_extent<T>)
            return static_values[i];
        else
            return dynamic_values[dynamic_index_f(i)];
    }

    template <auto I>
    constexpr auto operator[](
        std::integral_constant<decltype(I), I> i) const noexcept
    {
        if constexpr (static_values[i] != dynamic_extent<T>)
            return static_values[i];
        else
            return dynamic_values[dynamic_index<I>];
    }

    template <class U, U I>
    constexpr auto operator[](std::integral_constant<U, I>) noexcept
        -> std::enable_if_t<static_values[I] == dynamic_extent<T>, T&>
    {
        return dynamic_values[dynamic_index<I>];
    }

    template <size_t I>
    friend constexpr auto get(const semi_static_array& src) noexcept
    {
        return src[size_constant<I>{}];
    }

    friend constexpr void swap(
        semi_static_array& lhs, semi_static_array& rhs) noexcept
    {
        lhs.dynamic_values.swap(rhs.dynamic_values);
    }

    friend constexpr auto operator==(
        const semi_static_array& lhs, const semi_static_array& rhs) noexcept
    {
        return detail::equal_array(lhs.dynamic_values, rhs.dynamic_values);
    }

    friend constexpr auto operator!=(
        const semi_static_array& lhs, const semi_static_array& rhs) noexcept
    {
        return !(lhs == rhs);
    }

private:
    static constexpr auto indexes = std::make_index_sequence<size()>{};

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

template <
    class... Ts,
    enable_int_if<are_same_v<decltype(static_extent_v<Ts>)...>> = 0>
semi_static_array(Ts...) -> semi_static_array<
    std::common_type_t<decltype(static_extent_v<Ts>)...>,
    static_extent_v<Ts>...>;

template <class T, T... Values>
struct range_traits<semi_static_array<T, Values...>> : default_range_traits
{
    static constexpr index static_size = sizeof...(Values);
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
