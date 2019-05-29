/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/bit.hpp>
#include <actl/io/manip/skip.hpp>
#include <actl/io/text/flags.hpp>
#include <utility>

namespace ac::io {

const flag_t group_bits[] = {bit(flags::fixed) | bit(flags::scientific) | bit(flags::hexfloat),
                             bit(flags::left) | bit(flags::right) | bit(flags::center)};

template <
    class Char = char,
    flag_t Flags = bit(flags::skipws),
    uint8_t Base = 10,
    index Precision = 6,
    index Width = 0,
    Char Fill = ' '>
class text_static {
public:
    static constexpr mode_t mode = 0;

    static constexpr flag_t flags() { return Flags; }

    static constexpr bool getf(flag_t flag) { return has_bit(Flags, flag); }

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

    bool getf(flag_t flag) const { return has_bit(flags(), flag); }
    void setf(flag_t flag) { flags_ |= bit(flag); }
    void unsetf(flag_t flag) { flags_ = clear_bit(flags_, flag); }

    void setf(flag_t flag, flag_t group) {
        flags_ = set_bits(flags_, group_bits[group], bit(flag));
    }

    uint8_t base() const { return base_; }
    void base(uint8_t value) {
        ACTL_ASSERT(value == 0 || 1 < value && value <= 36);
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
inline constexpr std::pair<index, index> adjustment(Format& fmt, index size) {
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
inline index serialize(Device& id, Format& fmt, const span<const char_t<Device>>& s, text) {
    if (fmt.width() == 0 || fmt.width() <= s.size()) return id.write(s);
    auto [l, r] = adjustment(fmt, s.size());
    id.write_fill(fmt.fill(), l);
    index res = id.write(s);
    id.write_fill(fmt.fill(), r);
    return res + l + r;
}

template <class Device, class Format>
inline bool deserialize(Device& id, Format& fmt, char_t<Device>& c, text) {
    if (fmt.getf(flags::skipws)) read(id, fmt, ws);
    return deserialize(id, fmt, c);
}

// Use this class to write string representation of a non-string type.
// Distinguishes such string from spans passed by the user.
template <class Char>
class ospan : public span<const Char> {
public:
    using span<const Char>::span;

    template <index N>
    ospan(const Char (&array)[N]) : span<const Char>{array, array[N - 1] ? N : N - 1} {}
};

// Null-terminated string literals handling.
template <class Device, class Format, index N>
inline index serialize(Device& od, Format& fmt, const char_t<Device> (&array)[N], text) {
    return serialize(od, fmt, ospan{array});
}

template <class Device, class Format, index N>
inline bool deserialize(Device& od, Format& fmt, const char_t<Device> (&array)[N], text) {
    return deserialize(od, fmt, ospan{array});
}

}  // namespace ac::io
