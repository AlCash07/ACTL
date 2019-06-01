/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/bit.hpp>
#include <actl/io/manip/raw.hpp>
#include <actl/io/manip/skip.hpp>
#include <actl/io/text/flags.hpp>

namespace ac::io {

const flag_t group_bits[] = {bit(flags::fixed) | bit(flags::scientific) | bit(flags::hexfloat),
                             bit(flags::left) | bit(flags::right) | bit(flags::center)};

template <
    flag_t Flags = bit(flags::skipws),
    uint8_t Base = 10,
    index Precision = 6>
class text_static {
public:
    static constexpr mode_t mode = 0;

    static constexpr flag_t flags() { return Flags; }

    static constexpr bool getf(flag_t flag) { return has_bit(Flags, flag); }

    static constexpr uint8_t base() { return Base; }

    static constexpr index precision() { return Precision; }
};

template <flag_t F, uint8_t B, index P>
struct format_traits<text_static<F, B, P>> {
    using tag = text_tag;
};

class text {
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

    index precision() const { return precision_; }
    void precision(index value) {
        ACTL_ASSERT(0 <= value);
        precision_ = value;
    }

protected:
    using ts = text_static<>;

    flag_t flags_ = ts::flags();
    uint8_t base_ = ts::base();
    index precision_ = ts::precision();
};

template <>
struct format_traits<text> {
    using tag = text_tag;
};

template <class Device, class Format>
inline bool deserialize(Device& id, Format& fmt, char_t<Device>& c, text_tag) {
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
inline index serialize(Device& od, Format& fmt, const char_t<Device> (&array)[N], text_tag) {
    return serialize(od, fmt, ospan{array});
}

template <class Device, class Format, index N>
inline bool deserialize(Device& od, Format& fmt, const char_t<Device> (&array)[N], text_tag) {
    return deserialize(od, fmt, ospan{array});
}

template <class Device, class... Ts>
inline index writeln(Device&& od, Ts&&... args) {
    return write(od, args..., raw{'\n'});
}

}  // namespace ac::io
