/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/io/text/flags.hpp>
#include <actl/io/util/raw.hpp>
#include <actl/io/util/skip.hpp>
#include <actl/numeric/bit.hpp>
#include <string_view>

namespace ac::io {

template <class T, class Char = char>
inline constexpr bool is_string_v = std::is_convertible_v<T, std::basic_string_view<Char>>;

template <class Device>
using view_t = std::basic_string_view<char_t<Device>>;

const flag_t group_bits[] = {bit(flags::fixed) | bit(flags::scientific) | bit(flags::hexfloat),
                             bit(flags::left) | bit(flags::right) | bit(flags::center)};

template <
    flag_t Flags = bit(flags::skipws),
    uint8_t Base = 10,
    index Precision = 6>
class text_static {
public:
    using format_tag = text_tag;

    static constexpr flag_t flags() { return Flags; }

    static constexpr bool getf(flag_t flag) { return has_bit(Flags, flag); }

    static constexpr uint8_t base() { return Base; }

    static constexpr index precision() { return Precision; }
};

class text {
public:
    using format_tag = text_tag;

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

template <class Device, enable_int_if<!is_bin<Device::mode>> = 0>
inline text_static<> deduce_format(Device& dev) {
    return {};
}

template <class Device, class Format>
inline bool deserialize(Device& id, Format& fmt, char_t<Device>& c, text_tag) {
    if (fmt.getf(flags::skipws)) read(id, fmt, ws);
    return deserialize(id, fmt, c);
}

template <class Device, class Format>
inline bool deserialize(Device& id, Format& fmt, view_t<Device>& x, text_tag) {
    return deserialize(id, fmt, cspan<char_t<Device>>{x});
}

template <class Format, class S, enable_int_if_text<Format> = 0, enable_int_if<is_string_v<S>> = 0>
inline auto serialize(Format& fmt, const S& s) {
    return span{std::basic_string_view<value_t<S>>{s}};
}

template <class Device, class... Ts>
inline index writeln(Device&& od, Ts&&... args) {
    return write(od, args..., raw{'\n'});
}

}  // namespace ac::io
