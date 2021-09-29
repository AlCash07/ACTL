// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/conversion/integral_constant.hpp>
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
            return (... && can_convert_to_v<t_constant<Values>, Ts>);
        else
            return false;
    }

    static constexpr std::array<T, sizeof...(Values)> array = {Values...};

public:
    static constexpr size_t size() noexcept
    {
        return array.size();
    }

    static constexpr const T* data() noexcept
    {
        return array.data();
    }

    constexpr static_array() noexcept = default;

    // TODO: remove explicit first parameter
    // when std::is_trivial implementation is fixes in clang
    template <
        class T0,
        class... Ts,
        enable_int_if<can_construct_from<T0, Ts...>()> = 0>
    explicit constexpr static_array(T0 x0, Ts... xs) noexcept(
        ACTL_ASSERT_IS_NOEXCEPT())
    {
        check_values(x0, xs...);
    }

    using base_t::operator[];

    template <auto I>
    constexpr auto operator[](
        std::integral_constant<decltype(I), I>) const noexcept
    {
        static_assert(0 <= I && I < size());
        return t_constant<array[I]>{};
    }

    template <size_t I>
    constexpr auto get() const noexcept
    {
        return (*this)[t_constant<I>{}];
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

private:
    template <class... Ts>
    static constexpr void check_values(Ts... xs) noexcept(
        ACTL_ASSERT_IS_NOEXCEPT())
    {
        (..., convert_to<t_constant<Values>>(xs));
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
    using type = decltype(ac::static_array<T, Values...>{}.template get<I>());
};

} // namespace std
