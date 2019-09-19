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

template <class Tag>
struct adjusted_tag {
    using base = Tag;
};

template <class Format, class Char = char>
class adjusted : public Format {
public:
    using format_tag = adjusted_tag<typename Format::format_tag>;

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

template <class Device>
inline void write_fill(Device& od, char_t<Device> c, index count) {
    if constexpr (has_output_buffer<Device>::value) {
        auto s = od.output_data();
        if (count <= s.size()) {
            std::fill_n(s.data(), count, c);
            od.move(count);
        } else {
            std::fill_n(s.data(), s.size(), c);
            od.move(s.size());
            count -= s.size();
            s = od.output_data();
            std::fill_n(s.data(), std::min(count, s.size()), c);
            // Here we assume that s references device buffer and does not change.
            for (index n = count / s.size(); n > 0; --n) {
                od.move(s.size());
            }
            od.move(count % s.size());
        }
    } else {
        for (; 0 < count; --count) od.write(c);
    }
}

template <class Device, class Format, class Tag>
inline index serialize(Device& od, Format& fmt, const cspan<char_t<Device>>& s, adjusted_tag<Tag>) {
    if (fmt.width() <= 0 || fmt.width() <= s.size()) return od.write(s);
    auto [l, r] = adjustment(fmt, s.size());
    write_fill(od, fmt.fill(), l);
    index res = od.write(s);
    write_fill(od, fmt.fill(), r);
    return l + res + r;
}

template <class Device, class Format, class Tag>
inline index serialize(Device& od, Format& fmt, char_t<Device> c, adjusted_tag<Tag>) {
    return serialize(od, fmt, cspan<char_t<Device>>{&c, 1}, adjusted_tag<Tag>{});
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

template <class Device, class Format>
inline index serialize(Device&, Format& fmt, setwidth x) {
    fmt.width(x.value);
    return 0;
}

// character to pad units with less width
template <class Char>
struct setfill {
    explicit setfill(Char c) : value{c} {}

    Char value = ' ';

    struct is_manipulator;
};

template <class Device, class Format, class Char>
inline index serialize(Device&, Format& fmt, setfill<Char> x) {
    fmt.fill(x.value);
    return 0;
}

}  // namespace ac::io
