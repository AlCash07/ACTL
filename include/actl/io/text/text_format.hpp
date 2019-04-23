/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/io/io.hpp>
#include <actl/io/text/flags.hpp>

namespace ac::io {

inline constexpr flag_t bits(flag_t flag) { return flag_t{1} << flag; }

const flag_t group_bits[] = {bits(flags::fixed) | bits(flags::scientific) | bits(flags::hexfloat),
                             bits(flags::left) | bits(flags::right) | bits(flags::center)};

template <
    flag_t Flags = bits(flags::skipws),
    uint8_t Base = 10,
    width_t Precision = 6,
    width_t Width = 0,
    char Fill = ' '>
class text_static {
public:
    static constexpr mode_t mode = 0;

    static constexpr flag_t flags() { return Flags; }

    static constexpr bool getf(flag_t flag) { return (Flags & bits(flag)) != 0; }

    static constexpr uint8_t base() { return Base; }

    static constexpr width_t precision() { return Precision; }

    static constexpr width_t width() { return Width; }

    static constexpr char fill() { return Fill; }
};

template <flag_t Fl, uint8_t B, width_t P, width_t W, char F>
struct format_traits<text_static<Fl, B, P, W, F>> {
    using tag = text;
};

template <mode_t Mode, bool = is_out<Mode>>
class text_format {
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
        ACTL_ASSERT(value != 1 && value < 36);
        base_ = value;
    }

protected:
    flag_t flags_ = text_static<>::flags();
    uint8_t base_ = text_static<>::base();
};

template <mode_t Mode>
class text_format<Mode, true> : public text_format<Mode, false> {
public:
    width_t precision() const { return precision_; }
    void precision(width_t value) { precision_ = value; }

    width_t width() const { return width_; }
    void width(width_t value) { width_ = value; }

    char fill() const { return fill_; }
    void fill(char value) { fill_ = value; }

protected:
    using ts = text_static<>;

    width_t precision_ = ts::precision();
    width_t width_ = ts::width();
    char fill_ = ts::fill();
};

template <mode_t Mode, bool B>
struct format_traits<text_format<Mode, B>> {
    using tag = text;
};

}  // namespace ac::io
