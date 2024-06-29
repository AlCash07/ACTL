// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/conversion/convert_to.hpp>

namespace ac {

template <class T>
struct converted {
    T value;

    explicit constexpr converted(T x) noexcept : value{x} {}

    constexpr bool operator==(converted rhs) const noexcept {
        return value == rhs.value;
    }
};

template <class To, class From>
struct conversion<converted<To>, converted<From>> {
    static constexpr bool value = can_convert_to_v<To, From>;

    static constexpr converted<To> convert(converted<From> x) noexcept {
        return converted<To>{x.value};
    }
};

template <class To, class From>
struct conversion<converted<To>, converted<From> const&>
    : conversion<converted<To>, converted<From>> {};

static_assert(!std::is_constructible_v<converted<int>, converted<char>>);
static_assert(can_convert_to_v<converted<int>, converted<char>>);
static_assert(can_convert_to_v<converted<int>, converted<char> const&>);

} // namespace ac
