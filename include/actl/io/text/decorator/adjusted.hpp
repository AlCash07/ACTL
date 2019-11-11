/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/text/manip.hpp>
#include <actl/std/utility.hpp>

namespace ac::io {

template <class Char>
struct repeat {
    Char c;
    index count;
};

template <class Char = char>
class adjusted {
public:
    struct format_tag;

    index width() const { return width_; }
    void width(index value) {
        ACTL_ASSERT(0 <= value);
        width_ = value;
    }

    Char fill() const { return fill_; }
    void fill(Char value) { fill_ = value; }

protected:
    index width_ = 0;
    Char fill_ = ' ';
};

template <class Format>
inline constexpr std::pair<index, index> adjustment(Format& fmt, index size) {
    size = fmt.width() - size;
    if (size <= 0) return {0, 0};
    auto p =
        fmt.getf(flags::center) ? std::pair{size / 2, size - size / 2} : std::pair{index{}, size};
    return fmt.getf(flags::left) ? std::pair{p.second, p.first} : p;
}

template <class Device, class Format, class Char>
inline index write_final(Device& od, Format& fmt, const repeat<Char>& x) {
    index count = x.count;
    if constexpr (has_output_buffer<Device>::value) {
        auto s = od.output_data();
        if (count <= s.size()) {
            std::fill_n(s.data(), count, x.c);
            od.move(count);
        } else {
            std::fill_n(s.data(), s.size(), x.c);
            od.move(s.size());
            count -= s.size();
            s = od.output_data();
            std::fill_n(s.data(), std::min(count, s.size()), x.c);
            // Here we assume that s references device buffer and does not change.
            for (index n = count / s.size(); n > 0; --n) {
                od.move(s.size());
            }
            od.move(count % s.size());
        }
    } else {
        for (; 0 < count; --count) od.write(x.c);
    }
    return x.count;
}

template <class Char, class T>
inline tuple<repeat<Char>, const T&, repeat<Char>> serialize(adjusted<Char>& fmt, const T& x) {
    auto [l, r] = adjustment(fmt, x.size());
    return {repeat{fmt.fill, l}, x, repeat{fmt.fill, r}};
}

// adjustment
constexpr setg<groups::adjustfield, flags::left> left{};
constexpr setg<groups::adjustfield, flags::right> right{};
constexpr setg<groups::adjustfield, flags::center> center{};

// minimum width of an output unit
struct setwidth {
    index value = 0;

    struct is_manipulator;
};

template <class Char>
inline void serialize(adjusted<Char>& fmt, setwidth x) {
    fmt.width(x.value);
}

// character to pad units with less width
template <class Char>
struct setfill {
    explicit setfill(Char c) : value{c} {}

    Char value = ' ';

    struct is_manipulator;
};

template <class Char>
inline void serialize(adjusted<Char>& fmt, setfill<Char> x) {
    fmt.fill(x.value);
}

}  // namespace ac::io
