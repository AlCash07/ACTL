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
    using size_type = index;
};

} // namespace detail

template <class T, T... Values>
class static_array
    : public contiguous_range_facade<
          static_array<T, Values...>,
          detail::sa_types<T>>
{
    static constexpr std::array<T, sizeof...(Values)> array = {Values...};

public:
    constexpr static_array() noexcept = default;

    template <class R, enable_int_if<is_range_v<R>> = 0>
    explicit constexpr static_array(R&& range)
    {
        ACTL_ASSERT(equal(array, range));
    }

    static constexpr const T* data() noexcept
    {
        return array.data();
    }

    static constexpr index size() noexcept
    {
        return index{array.size()};
    }

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

    template <T... OtherValues>
    friend constexpr std::false_type operator==(
        static_array, static_array<T, OtherValues...>) noexcept
    {
        return {};
    }

    template <T... OtherValues>
    friend constexpr std::true_type operator!=(
        static_array, static_array<T, OtherValues...>) noexcept
    {
        return {};
    }
};

template <class T, T... Values>
struct range_traits<static_array<T, Values...>> : default_range_traits
{
    static constexpr index static_size = sizeof...(Values);
};

} // namespace ac
