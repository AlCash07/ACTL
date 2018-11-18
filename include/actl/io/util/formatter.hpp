/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <cstdint>

namespace ac {

class formatter {
public:
    using flags_t = uint32_t;
    static const flags_t boolalpha   = 0x0001;
    static const flags_t showbase    = 0x0002;
    static const flags_t showpos     = 0x0004;
    static const flags_t uppercase   = 0x0008;
    static const flags_t fixed       = 0x0010;
    static const flags_t scientific  = 0x0020;
    static const flags_t showpoint   = 0x0040;
    static const flags_t left        = 0x0000;
    static const flags_t right       = 0x0100;
    static const flags_t center      = 0x0200;
    static const flags_t internal    = 0x0300;
    static const flags_t skipws      = 0x1000;
    static const flags_t unitbuf     = 0x2000;
    static const flags_t brackets    = 0x4000;
    static const flags_t floatfield  = fixed | scientific;
    static const flags_t adjustfield = left | right | center | internal;

    flags_t flags() const { return flags_; }
    void flags(flags_t value) { flags_ = value; }
    void setf(flags_t value) { flags_ |= value; }
    void setf(flags_t value, flags_t mask) {
        unsetf(mask);
        flags_ |= value & mask;
    }
    void unsetf(flags_t mask) { flags_ &= ~mask; }

    uint8_t base() const { return base_; }
    void base(uint8_t value) { base_ = value; }

    uint8_t precision() const { return precision_; }
    void precision(uint8_t value) { precision_ = value; }

    uint8_t width() const { return width_; }
    void width(uint8_t value) { width_ = value; }

    char fill() const { return fill_; }
    void fill(char value) { fill_ = value; }

protected:
    uint32_t flags_ = right | skipws;
    uint8_t base_ = 10;
    uint8_t precision_ = 6;
    uint8_t width_ = 0;
    char fill_ = ' ';
};

}  // namespace ac
