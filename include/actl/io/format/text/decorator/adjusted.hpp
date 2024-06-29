// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/batch.hpp>
#include <actl/io/utility/repeat.hpp>
#include <actl/std/utility.hpp>

namespace ac::io {

enum class adjust_to : uint8_t {
    left = 0,
    right = 1,
    center = 2,
    center_right = 3
};

template <class Char = char>
struct adjusted {
    explicit constexpr adjusted(
        size_t width, adjust_to where = adjust_to::left, Char fill = ' '
    )
        : width{width}, where{where}, fill{fill} {}

    size_t width;
    adjust_to where;
    Char fill;

    struct format_tag;
};

template <class Format>
constexpr std::pair<size_t, size_t> adjustment(Format const& fmt, size_t size) {
    if (fmt.width <= size)
        return {0, 0};
    size = static_cast<size_t>(fmt.width - size);
    auto p = (static_cast<uint8_t>(fmt.where) &
              static_cast<uint8_t>(adjust_to::center)) != 0
                 ? std::pair{size / 2, size - size / 2}
                 : std::pair{size_t{}, size};
    return (static_cast<uint8_t>(fmt.where) &
            static_cast<uint8_t>(adjust_to::right)) != 0
               ? std::pair{p.second, p.first}
               : p;
}

template <class Char, class T>
    requires std::is_constructible_v<cspan<Char>, T>
auto encode(adjusted<Char> const& fmt, T&& x) {
    auto [l, r] = adjustment(fmt, cspan<Char>{x}.size());
    return batch{repeat{fmt.fill, l}, std::forward<T>(x), repeat{fmt.fill, r}};
}

template <class Char>
auto encode(adjusted<Char> const& fmt, Char x) {
    auto [l, r] = adjustment(fmt, 1);
    return batch{repeat{fmt.fill, l}, std::move(x), repeat{fmt.fill, r}};
}

} // namespace ac::io
