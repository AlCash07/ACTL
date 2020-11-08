/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/util/repeat.hpp>
#include <actl/std/utility.hpp>

namespace ac::io {

enum class adjust_to : uint8_t { left = 0, right = 1, center = 2, center_right = 3 };

template <class Char = char>
struct adjusted {
    struct format_tag;

    index width = 0;
    adjust_to where = adjust_to::left;
    Char fill = ' ';
};

template <class Format>
inline constexpr std::pair<index, index> adjustment(Format& fmt, index size) {
    size = fmt.width - size;
    if (size <= 0) return {0, 0};
    auto p = (static_cast<uint8_t>(fmt.where) & static_cast<uint8_t>(adjust_to::center)) != 0
                 ? std::pair{size / 2, size - size / 2}
                 : std::pair{index{}, size};
    return (static_cast<uint8_t>(fmt.where) & static_cast<uint8_t>(adjust_to::right)) != 0
               ? std::pair{p.second, p.first}
               : p;
}

template <class Char, class T, enable_int_if<std::is_constructible_v<cspan<Char>, T>> = 0>
auto encode(adjusted<Char>& fmt, const T& x) {
    auto [l, r] = adjustment(fmt, cspan<Char>{x}.size());
    return batch{repeat{fmt.fill, l}, x, repeat{fmt.fill, r}};
}

template <class Char>
auto encode(adjusted<Char>& fmt, Char x) {
    auto [l, r] = adjustment(fmt, 1);
    return batch{repeat{fmt.fill, l}, std::move(x), repeat{fmt.fill, r}};
}

/* Manipulators */

template <>
struct is_manipulator<adjust_to> : std::true_type {};

template <class Char>
inline void manipulate(adjusted<Char>& fmt, adjust_to x) {
    fmt.where = x;
}

// minimum width of an output unit
struct setwidth {
    index value = 0;

    struct is_manipulator;
};

template <class Char>
inline void manipulate(adjusted<Char>& fmt, setwidth x) {
    fmt.width = x.value;
}

// character to pad units with less width
template <class Char>
struct setfill {
    explicit setfill(Char c) : value{c} {}

    Char value = ' ';

    struct is_manipulator;
};

template <class Char>
inline void manipulate(adjusted<Char>& fmt, setfill<Char> x) {
    fmt.fill = x.value;
}

}  // namespace ac::io
