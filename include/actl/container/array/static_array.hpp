// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/facade/contiguous_range_facade.hpp>
#include <actl/std/array.hpp>

namespace ac {

namespace detail {

template <class T>
struct sa_types
{
    using iterator = const T*;
    using size_type = size_t;
};

} // namespace detail

template <class T, T... Values>
class static_array
    : public contiguous_range_facade<
          static_array<T, Values...>,
          detail::sa_types<T>>
{
    using base_t = contiguous_range_facade<
        static_array<T, Values...>,
        detail::sa_types<T>>;

    template <T Value>
    using t_constant = std::integral_constant<T, Value>;

    template <class... Ts>
    static constexpr bool can_construct_from()
    {
        if constexpr (sizeof...(Ts) == sizeof...(Values))
            return (... && std::is_same_v<t_constant<Values>, Ts>);
        else
            return false;
    }

    static constexpr std::array<T, sizeof...(Values)> array = {Values...};

public:
    static constexpr auto size() noexcept
    {
        return std::integral_constant<size_t, array.size()>{};
    }

    static constexpr const T* data() noexcept
    {
        return array.data();
    }

    constexpr static_array() noexcept = default;

    // TODO: when std::is_trivial implementation is fixes in clang,
    // this can be simplified to static_array(t_constant<Values>...)
    template <
        class T0,
        class... Ts,
        enable_int_if<can_construct_from<T0, Ts...>()> = 0>
    constexpr static_array(T0, Ts...) noexcept
    {}

    using base_t::operator[];

    template <auto I>
    constexpr auto operator[](
        std::integral_constant<decltype(I), I>) const noexcept
    {
        static_assert(0 <= I && I < size());
        return t_constant<array[I]>{};
    }

    template <size_t I>
    friend constexpr auto get(static_array src) noexcept
    {
        return src[t_constant<I>{}];
    }

    friend constexpr void swap(static_array&, static_array&) noexcept {}

    friend constexpr std::true_type operator==(
        static_array, static_array) noexcept
    {
        return {};
    }

    friend constexpr std::false_type operator!=(
        static_array, static_array) noexcept
    {
        return {};
    }
};

template <class T, T... Values>
static_array(std::integral_constant<T, Values>...)
    -> static_array<T, Values...>;

template <class T, T... Values>
struct range_traits<static_array<T, Values...>> : default_range_traits
{
    static constexpr index static_size = sizeof...(Values);
};

} // namespace ac

namespace std {

template <class T, T... Values>
struct tuple_size<ac::static_array<T, Values...>>
{
    static constexpr size_t value = sizeof...(Values);
};

template <size_t I, class T, T... Values>
struct tuple_element<I, ac::static_array<T, Values...>>
{
    using type = decltype(get<I>(ac::static_array<T, Values...>{}));
};

} // namespace std
