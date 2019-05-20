/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/io/manip/skip.hpp>
#include <actl/io/text/flags.hpp>
#include <utility>

namespace ac::io {

inline constexpr flag_t bits(flag_t flag) { return flag_t{1} << flag; }

const flag_t group_bits[] = {bits(flags::fixed) | bits(flags::scientific) | bits(flags::hexfloat),
                             bits(flags::left) | bits(flags::right) | bits(flags::center)};

template <
    class Char = char,
    flag_t Flags = bits(flags::skipws),
    uint8_t Base = 10,
    index Precision = 6,
    index Width = 0,
    Char Fill = ' '>
class text_static {
public:
    static constexpr mode_t mode = 0;

    static constexpr flag_t flags() { return Flags; }

    static constexpr bool getf(flag_t flag) { return (Flags & bits(flag)) != 0; }

    static constexpr uint8_t base() { return Base; }

    static constexpr index precision() { return Precision; }

    static constexpr index width() { return Width; }

    static constexpr Char fill() { return Fill; }
};

template <class C, flag_t Fl, uint8_t B, index P, index W, C F>
struct format_traits<text_static<C, Fl, B, P, W, F>> {
    using tag = text;
};

template <class Char>
class in_text {
public:
    flag_t flags() const { return flags_; }
    void flags(flag_t value) { flags_ = value; }

    bool getf(flag_t flag) const { return (flags() & bits(flag)) != 0; }
    void setf(flag_t flag) { flags_ |= bits(flag); }
    void unsetf(flag_t flag) { flags_ &= ~bits(flag); }

    void setf(flag_t flag, flag_t group) {
        group = group_bits[group];
        flags_ = (flags_ & ~group) | (bits(flag) & group);
    }

    uint8_t base() const { return base_; }
    void base(uint8_t value) {
        ACTL_ASSERT(1 < value && value <= 36);
        base_ = value;
    }

protected:
    using ts = text_static<Char>;

    flag_t flags_ = ts::flags();
    uint8_t base_ = ts::base();
};

template <class C>
struct format_traits<in_text<C>> {
    using tag = text;
};

template <class Char>
class out_text : public in_text<Char> {
public:
    index precision() const { return precision_; }
    void precision(index value) {
        ACTL_ASSERT(0 <= value);
        precision_ = value;
    }

    index width() const { return width_; }
    void width(index value) {
        ACTL_ASSERT(0 <= value);
        width_ = value;
    }

    Char fill() const { return fill_; }
    void fill(Char value) { fill_ = value; }

protected:
    using ts = text_static<Char>;

    index precision_ = ts::precision();
    index width_ = ts::width();
    Char fill_ = ts::fill();
};

template <class C>
struct format_traits<out_text<C>> {
    using tag = text;
};

template <mode_t Mode, class Char = default_char_t<Mode>>
using text_format = std::conditional_t<is_out<Mode>, out_text<Char>, in_text<Char>>;

template <class Format>
inline std::pair<index, index> adjustment(Format& fmt, index size) {
    size = fmt.width() - size;
    if (size <= 0) return {0, 0};
    auto p = fmt.getf(flags::center) ? std::pair{size / 2, size - size / 2} : std::pair{index{}, size};
    return fmt.getf(flags::left) ? std::pair{p.second, p.first} : p;
}

template <class Device, class Format>
inline index serialize(Device& id, Format& fmt, char_t<Device> c, text) {
    return write(id, fmt, span<const char_t<Device>>{&c, 1});
}

template <class Device, class Format>
inline index serialize(Device& id, Format& fmt, span<const char_t<Device>> s, text) {
    auto [l, r] = adjustment(fmt, s.size());
    id.write_fill(fmt.fill(), l);
    index res = id.write(s);
    id.write_fill(fmt.fill(), r);
    return res + l + r;
}

template <class Device, class Format>
inline bool deserialize(Device& id, Format& fmt, char_t<Device>& c, text) {
    if (fmt.getf(flags::skipws) && !read(id, fmt, ws)) return false;
    return deserialize(id, fmt, c);
}

}  // namespace ac::io
