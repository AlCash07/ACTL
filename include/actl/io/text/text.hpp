// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/text/settings/precision.hpp>
#include <actl/io/util/raw.hpp>
#include <actl/numeric/bit.hpp>
#include <actl/string/traits.hpp>

namespace ac::io {

struct text_tag {};

template <class T>
using enable_int_if_text = enable_int_if<has_format_tag<T, text_tag>::value>;

using flag_t = uint32_t;

namespace flag {

enum : flag_t {
    boolalpha = 1 << 0,
    uppercase = 1 << 1,
    showbase = 1 << 2,
    showpos = 1 << 3,
    showpoint = 1 << 4,
    fixed = 1 << 5,
    scientific = 1 << 6,
    hexfloat = 1 << 7
};

}  // namespace flag

namespace groups {

enum : flag_t { floatfield };

}  // namespace groups

// Integer and real numbers base.
class base_t {
public:
    struct is_manipulator;

    explicit constexpr base_t() = default;

    template <class T>
    explicit constexpr base_t(T x) : value_{static_cast<uint8_t>(x)} {
        ACTL_ASSERT(x == 0 || 1 < x && x <= 36);
    }

    constexpr operator uint8_t() const { return value_; }

private:
    uint8_t value_ = 10;
};

const flag_t group_bits[] = {flag::fixed | flag::scientific | flag::hexfloat};

template <flag_t Flags = 0, uint8_t Base = 10, index Precision = 6>
class text_static {
public:
    using format_tag = text_tag;

    static constexpr flag_t flags() { return Flags; }

    static constexpr bool getf(flag_t flag) {
        return has_bits(Flags, flag);
    }

    static constexpr base_t base = base_t{Base};

    static constexpr precision_t precision = precision_t{Precision};
};

class text {
public:
    using format_tag = text_tag;

    flag_t flags() const { return flags_; }
    void flags(flag_t value) { flags_ = value; }

    bool getf(flag_t flag) const {
        return has_bits(flags(), flag);
    }
    void setf(flag_t flag) {
        flags_ |= flag;
    }
    void unsetf(flag_t flag) {
        flags_ = clear_bits(flags_, flag);
    }

    void setf(flag_t flag, flag_t group) {
        flags_ = set_bits(flags_, group_bits[group], flag);
    }

    base_t base;
    precision_t precision;

protected:
    flag_t flags_ = text_static<>::flags();
};

template <class Device, enable_int_if<!is_bin<Device::mode>> = 0>
text_static<> deduce_format(Device& dev) {
    return {};
}

template <class Format, class S, enable_int_if_text<Format> = 0, enable_int_if<is_string_v<S>> = 0>
auto encode(Format& fmt, const S& s) {
    return span{std::basic_string_view<value_t<S>>{s}};
}

template <class Device, class... Ts>
index writeln(Device&& od, Ts&&... args) {
    return write(od, args..., raw{'\n'});
}

/* I/O manipulators analogous to https://en.cppreference.com/w/cpp/io/manip */

template <flag_t Flag, bool Value>
struct setf {
    struct is_manipulator;
};

template <flag_t Group, flag_t Flag>
struct setg {
    struct is_manipulator;
};

// boolean as string or int
constexpr setf<flag::boolalpha, true> boolalpha{};
constexpr setf<flag::boolalpha, false> noboolalpha{};

// prepend integer number base prefix
constexpr setf<flag::showbase, true> showbase{};
constexpr setf<flag::showbase, false> noshowbase{};

// prepend '+' before positive integer and real numbers
constexpr setf<flag::showpos, true> showpos{};
constexpr setf<flag::showpos, false> noshowpos{};

// case for special characters in integer and real numbers representation
constexpr setf<flag::uppercase, true> uppercase{};
constexpr setf<flag::uppercase, false> nouppercase{};

// real numbers format
constexpr setg<groups::floatfield, 0> defaultfloat{};
constexpr setg<groups::floatfield, flag::fixed> fixed{};
constexpr setg<groups::floatfield, flag::scientific> scientific{};
constexpr setg<groups::floatfield, flag::hexfloat> hexfloat{};

// always show decimal point
constexpr setf<flag::showpoint, true> showpoint{};
constexpr setf<flag::showpoint, false> noshowpoint{};

template <class Format, flag_t Flag, bool Value>
void manipulate(Format& fmt, setf<Flag, Value>) {
    if constexpr (Value)
        fmt.setf(Flag);
    else
        fmt.unsetf(Flag);
}

template <class Format, flag_t Group, flag_t Flag>
void manipulate(Format& fmt, setg<Group, Flag>) {
    fmt.setf(Flag, Group);
}

using setbase = base_t;
constexpr setbase dec{10};
constexpr setbase hex{16};
constexpr setbase oct{8};

template <class Format>
void manipulate(Format& fmt, setbase x) {
    fmt.base = x;
}

}  // namespace ac::io
