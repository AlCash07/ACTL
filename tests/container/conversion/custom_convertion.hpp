// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/conversion/convert_to.hpp>

namespace ac {

template<typename T>
struct converted {
    T value;

    explicit constexpr converted(T x) noexcept : value{x} {}

    constexpr bool operator==(converted that) const noexcept {
        return this->value == that.value;
    }
};

template<typename Target, typename Source>
struct conversion<converted<Target>, converted<Source>> {
    static constexpr bool value = can_convert_to_v<Target, Source>;

    static constexpr converted<Target> convert(converted<Source> source
    ) noexcept {
        return converted<Target>{source.value};
    }
};

template<typename Target, typename Source>
struct conversion<converted<Target>, converted<Source> const&>
    : conversion<converted<Target>, converted<Source>> {};

static_assert(!std::is_constructible_v<converted<int>, converted<char>>);
static_assert(can_convert_to_v<converted<int>, converted<char>>);
static_assert(can_convert_to_v<converted<int>, converted<char> const&>);

} // namespace ac
