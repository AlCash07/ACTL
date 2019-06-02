/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/text/manip.hpp>
#include <utility>

namespace ac::io {

template <class Format, class Char = char>
class adjusted : public Format {
public:
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

template <class Tag>
struct adjusted_tag {
    using base = Tag;
};

template <class Format>
struct format_traits<adjusted<Format>> {
    using tag = adjusted_tag<format_tag_t<Format>>;
};

template <class Format>
inline constexpr std::pair<index, index> adjustment(Format& fmt, index size) {
    size = fmt.width() - size;
    if (size <= 0) return {0, 0};
    auto p =
        fmt.getf(flags::center) ? std::pair{size / 2, size - size / 2} : std::pair{index{}, size};
    return fmt.getf(flags::left) ? std::pair{p.second, p.first} : p;
}

template <class Device, class Format, class Tag>
inline index serialize(Device& od, Format& fmt, const span<const char_t<Device>>& s,
                       adjusted_tag<Tag>) {
    if (fmt.width() <= 0 || fmt.width() <= s.size()) return od.write(s);
    auto [l, r] = adjustment(fmt, s.size());
    od.write_fill(fmt.fill(), l);
    index res = od.write(s);
    od.write_fill(fmt.fill(), r);
    return l + res + r;
}

template <class Device, class Format, class Tag>
inline index serialize(Device& od, Format& fmt, char_t<Device> c, adjusted_tag<Tag>) {
    return serialize(od, fmt, span<const char_t<Device>>{&c, 1}, adjusted_tag<Tag>{});
}

// adjustment
constexpr setg<groups::adjustfield, flags::left> left{};
constexpr setg<groups::adjustfield, flags::right> right{};
constexpr setg<groups::adjustfield, flags::center> center{};

// minimum width of an output unit
struct setwidth {
    index value = 0;
};

template <>
struct is_manipulator<setwidth> : std::true_type {};

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
};

template <class C>
struct is_manipulator<setfill<C>> : std::true_type {};

template <class Device, class Format, class Char>
inline index serialize(Device&, Format& fmt, setfill<Char> x) {
    fmt.fill(x.value);
    return 0;
}

}  // namespace ac::io
