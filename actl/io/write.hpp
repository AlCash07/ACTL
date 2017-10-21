/***************************************************************************************************
 * Function write returns number of characters written.
 * Arguments supported by default:
 * - char, standard integer and floating point types
 * - range, string
 * Two consecutive arguments starting with an iterator are treated as a range:
 * - Iterator first, Iterator last
 * - Iterator first, Integer count
 * Function write to the text device writes strings until the null character is reached, so
 * const char* arguments are processed one by one, not grouped into ranges.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io_traits.hpp>
#include <actl/range/range.hpp>
#include <actl/type_traits/is_iterator.hpp>
#include <cmath>
#include <string>

namespace ac {

template <class Device>
inline int write(Device& out) { return 0; }

/* Arithmetic types */

template <class Device>
inline int write(Device& out, char arg) {
    out.write_char(arg);
    return 1;
}

template <class Device, class T>
inline std::enable_if_t<std::is_arithmetic<T>::value, int> write(binary_io_tag, Device& out,
                                                                 T arg) {
    return out.write_string(reinterpret_cast<const char*>(&arg), sizeof(T));
}

namespace detail {

template <class Device, class UInt>
inline char* write_unsigned_int(Device& out, UInt arg, char* last) {
    auto base = out.get_base();
    if (arg == 0) {
        *--last = '0';
    } else if (EXPECT_TRUE(base == 10)) {
        for (; arg; arg /= 10)
            *--last = static_cast<char>('0' + arg % 10);
    } else {
        for (; arg; arg /= base) {
            UInt digit = arg % base;
            *--last = static_cast<char>(digit < 10 ? '0' + digit : 'A' - 10 + digit);
        }
    }
    return last;
}

}  // namespace detail

template <class Device, class UInt>
inline std::enable_if_t<is_unsigned_int<UInt>::value, int> write(text_io_tag, Device& out,
                                                                 UInt arg) {
    return out.write_value(detail::write_unsigned_int(out, arg, out.get_end()));
}

template <class Device, class Int>
inline std::enable_if_t<is_signed_int<Int>::value, int> write(text_io_tag, Device& out, Int arg) {
    char* last = out.get_end();
    auto unsigned_arg = static_cast<std::make_unsigned_t<Int>>(arg);
    if (arg < 0) {
        last = detail::write_unsigned_int(out, ~unsigned_arg + 1, last);
        *--last = '-';
    } else {
        last = detail::write_unsigned_int(out, unsigned_arg, last);
    }
    return out.write_value(last);
}

template <class Device, class Float>
inline std::enable_if_t<std::is_floating_point<Float>::value, int> write(text_io_tag, Device& out,
                                                                         Float arg) {
    char* last = out.get_end();
    bool negative = std::signbit(arg);
    if (negative) arg = -arg;
    if (std::isnan(arg)) {
        for (int i = 0; i < 3; ++i)
            *--last = "NaN"[i];
    } else if (std::isinf(arg)) {
        for (int i = 0; i < 3; ++i)
            *--last = "fnI"[i];
    } else {
        auto base_power = out.get_base_power();
        auto integer_part = static_cast<unsigned long long>(arg);
        auto fractional_part =
            static_cast<unsigned long long>((arg - integer_part) * base_power + Float{1} / 2);
        if (fractional_part >= base_power) {
            ++integer_part;
            fractional_part = 0;
        }
        char* point = last - out.get_precision();
        if (out.get_precision() > 0) {
            std::fill(point, detail::write_unsigned_int(out, fractional_part, last), '0');
            *--point = '.';
        }
        last = detail::write_unsigned_int(out, integer_part, point);
    }
    if (negative) *--last = '-';
    return out.write_value(last);
}

/* Strings and ranges */

template <class Device>
inline int write(binary_io_tag, Device& out, const std::string& arg) {
    return write(out, static_cast<uint32_t>(arg.size()), arg.data(), arg.size());
}

template <class Device>
inline int write(text_io_tag, Device& out, const std::string& arg) {
    return out.write_string(arg.data(), arg.size(), false);
}

template <class Device, class... Ts>
inline int write(text_io_tag, Device& out, const char* arg, Ts&&... args) {
    return out.write_string(arg, std::strlen(arg), false) + write(out, std::forward<Ts>(args)...);
}

template <class Device, int N, class... Ts>
inline int write(text_io_tag, Device& out, char (&arg)[N], Ts&&... args) {
    return out.write_string(arg, std::strlen(arg), false) + write(out, std::forward<Ts>(args)...);
}

template <class Device, class T, class... Ts>
inline int write(binary_io_tag, Device& out, const range<T>& arg, Ts&&... args) {
    int chars_written = 0;
    for (const auto& element : arg)
        chars_written += write(out, element);
    return chars_written + write(out, std::forward<Ts>(args)...);
}

template <class Device, class T, class... Ts>
inline int write(text_io_tag, Device& out, const range<T>& arg, Ts&&... args) {
    int chars_written = 0;
    bool is_first = true;
    for (const auto& i : arg) {
        if (!is_first) chars_written += out.write_delimiter();
        is_first = false;
        chars_written += write(out, i);
    }
    return chars_written + write(out, std::forward<Ts>(args)...);
}

/* General forwarding */

template <class Device, class T0, class T1, class... Ts>
inline std::enable_if_t<is_iterator<T0>::value, int> write(io_tag, Device& out, T0&& arg0,
                                                           T1&& arg1, Ts&&... args) {
    return write(out, make_range(std::forward<T0>(arg0), std::forward<T1>(arg1)),
                 std::forward<Ts>(args)...);
}

template <class Device, class T0, class T1, class... Ts>
inline std::enable_if_t<!is_iterator<T0>::value, int> write(io_tag, Device& out, const T0& arg0,
                                                            T1&& arg1, Ts&&... args) {
    int chars_written = write(out, arg0);
    return chars_written + write(out, std::forward<T1>(arg1), std::forward<Ts>(args)...);
}

template <class Device, class... Ts>
inline int write(Device& out, Ts&&... args) {
    static_assert(!std::is_base_of<io_tag, Device>::value, "no matching call for write found");
    return write(typename Device::category{}, out, std::forward<Ts>(args)...);
}

template <class Device, class... Ts>
inline int writeln(Device& out, Ts&&... args) {
    static_assert(io_traits<Device>::text, "text device expected");
    return write(out, std::forward<Ts>(args)..., '\n');
}

}  // namespace ac
