/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <cstdint>
#include <string>

namespace ac::io {

using flag_t  = uint32_t;
using width_t = uint32_t;

namespace flags {

enum : flag_t {
    boolalpha,
    showbase,
    showpos,
    uppercase,
    fixed,
    scientific,
    hexfloat,
    showpoint,
    left,
    right,
    center,
    internal,
    skipws,
    unitbuf,
    brackets
};

}  // namespace flags

namespace groups {

enum : flag_t { floatfield, adjustfield };

}  // namespace groups

inline constexpr flag_t bits(flag_t flag) { return flag_t{1} << flag; }

const flag_t gbits[] = {
    bits(flags::fixed) | bits(flags::scientific) | bits(flags::hexfloat),
    bits(flags::left) | bits(flags::right) | bits(flags::center) | bits(flags::internal)};

const char space[] = " ";

template <
    flag_t Flags = bits(flags::skipws),
    uint8_t Base = 10,
    width_t Precision = 6,
    width_t Width = 0,
    char Fill = ' ',
    const char* Delimiter = space>
class static_format {
public:
    static constexpr flag_t flags() { return Flags; }

    static constexpr bool getf(flag_t flag) { return (Flags & bits(flag)) != 0; }

    static constexpr uint8_t base() { return Base; }

    static constexpr width_t precision() { return Precision; }

    static constexpr width_t width() { return Width; }

    static constexpr char fill() { return Fill; }

    static constexpr const char* delimiter() { return Delimiter; }
};

class format {
public:
    flag_t flags() const { return flags_; }
    void flags(flag_t value) { flags_ = value; }

    bool getf(flag_t flag) const { return (flags() & bits(flag)) != 0; }
    void setf(flag_t flag) { flags_ |= bits(flag); }
    void unsetf(flag_t flag) { flags_ &= ~bits(flag); }

    void setf(flag_t flag, flag_t group) {
        group = gbits[group];
        flags_ = (flags_ & ~group) | (bits(flag) & group);
    }

    uint8_t base() const { return base_; }
    void base(uint8_t value) { base_ = value; }

    width_t precision() const { return precision_; }
    void precision(uint8_t value) { precision_ = value; }

    width_t width() const { return width_; }
    void width(uint8_t value) { width_ = value; }

    char fill() const { return fill_; }
    void fill(char value) { fill_ = value; }

    const char* delimiter() const { return delimiter_.data(); }
    void delimiter(const char* value) { delimiter_ = value; }

protected:
    using sf = static_format<>;

    flag_t flags_ = sf::flags();
    width_t precision_ = sf::precision();
    width_t width_ = sf::width();
    uint8_t base_ = sf::base();
    char fill_ = sf::fill();
    std::string delimiter_ = sf::delimiter();
};

template <class Device, class Format = format>
class formatted : public Device, public Format {
public:
    using Device::Device;
};

}  // namespace ac::io
