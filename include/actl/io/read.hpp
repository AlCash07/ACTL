/***************************************************************************************************
 * Function read returns true if all the arguments were successfully read.
 * Arguments supported by default:
 * - char, standard integer and floating point types
 * - range, string
 * Two consecutive arguments starting with an output iterator are treated as a range:
 * - Iterator first, Iterator last
 * - Iterator first, Integer count
 * Function read from the text device additionally supports characters skip:
 * - char: skip until the given character is encountered and read it
 * - const char*: skip all the characters from the string
 * - predicate: skip all the characters satisfying the predicate
 * Character ranges from the text device are treated differently:
 * they are read until the range size is reached or termination character is found
 * (can be one of the characters in a given string or defined by predicate, isspace by default)
 * - char (&)[N], terminator
 * - iterator_range<char>, terminator
 * - string&, terminator
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io_traits.hpp>
#include <actl/macros.hpp>
#include <actl/range/iterator_range.hpp>
#include <actl/traits/ctype.hpp>
#include <actl/traits/iterator_traits.hpp>
#include <actl/traits/type_traits.hpp>
#include <cstring>
#include <string>

namespace ac {

template <class Device>
inline bool read(Device&) {
    return true;
}

/* Arithmetic types */

template <class Device>
inline bool read(Device& in, char& arg) {
    if (in.eof()) {
        arg = '\0';
        return false;
    } else {
        arg = in.get();
        return true;
    }
}

template <class Device, class T>
inline std::enable_if_t<std::is_arithmetic_v<T>, bool> read(binary_io_tag, Device& in, T& arg) {
    return in.read_string(reinterpret_cast<char*>(&arg), sizeof(T)) == sizeof(T);
}

namespace detail {

template <class Device, class UInt>
inline int read_unsigned_int_general(Device& in, UInt& arg, char c) {
    auto base   = in.get_base();
    UInt value  = 0;
    int  length = 0;
    for (;; c = in.get()) {
        if (is_digit(c)) c -= '0';
        else if (is_upper(c)) c -= 'A' - 10;
        else if (is_lower(c)) c -= 'a' - 10;
        else c = static_cast<char>(base);
        if (c >= base) break;
        value = base * value + c;
        ++length;
    }
    in.unget();
    arg = value;
    return length;
}

template <class Device, class Int>
inline int read_unsigned_int(Device& in, Int& arg, char c) {
    auto base = in.get_base();
    if (EXPECT_FALSE(base > 10)) return read_unsigned_int_general(in, arg, c);
    Int value  = 0;
    int length = 0;
    for (; static_cast<unsigned char>(c - '0') < base; c = in.get()) {
        value = base * value + c - '0';
        ++length;
    }
    in.unget();
    arg = value;
    return length;
}

inline bool is_one_of(char c, const char* str) { return std::strchr(str, c) != nullptr; }

template <class Device, class UnaryPredicate>
inline char skip_characters(Device& in, UnaryPredicate is_skipped) {
    char c;
    do { c = in.get(); } while (is_skipped(c));
    return c;
}

template <class Device>
inline char skip_characters(Device& in) {
    return skip_characters(in, is_space);
}

template <class Device>
inline int read_string(Device& in, char* arg, int limit, const char& terminator) {
    return in.read_string(arg, limit, [terminator](char c) { return c == terminator; });
}

template <class Device>
inline int read_string(Device& in, char* arg, int limit, const char* terminators) {
    if (!*terminators) return in.read_string(arg, limit, is_space);
    return in.read_string(arg, limit, [terminators](char c) { return is_one_of(c, terminators); });
}

template <class Device, class UnaryPredicate>
inline int read_string(Device& in, char* arg, int limit, UnaryPredicate is_terminator) {
    return in.read_string(arg, limit, is_terminator);
}

}  // namespace detail

template <class Device, class UInt>
inline std::enable_if_t<is_unsigned_int_v<UInt>, bool> read(text_io_tag, Device& in, UInt& arg) {
    return detail::read_unsigned_int(in, arg, detail::skip_characters(in)) > 0;
}

template <class Device, class Int>
inline std::enable_if_t<is_signed_int_v<Int> && !std::is_same_v<Int, char>, bool>
read(text_io_tag, Device& in, Int& arg) {
    char c = detail::skip_characters(in);
    bool negative = c == '-';
    if (negative) c = in.get();
    std::make_unsigned_t<Int> unsigned_arg;
    if (detail::read_unsigned_int(in, unsigned_arg, c) == 0) return false;
    arg = negative ? ~static_cast<Int>(unsigned_arg - 1) : static_cast<Int>(unsigned_arg);
    return true;
}

template <class Device, class Float>
inline std::enable_if_t<std::is_floating_point_v<Float>, bool> read(text_io_tag, Device& in,
                                                                    Float& arg) {
    char c = detail::skip_characters(in);
    bool negative = c == '-';
    if (negative) c = in.get();
    unsigned long long integer_part;
    if (detail::read_unsigned_int(in, integer_part, c) == 0) return false;
    arg = static_cast<Float>(integer_part);
    if (in.get() == '.') {
        unsigned long long fractional_part = 0;
        int fractional_length = detail::read_unsigned_int(in, fractional_part, in.get());
        if (fractional_length > 0) {
            unsigned long long base_power = 1;
            auto base = in.get_base();
            for (; fractional_length; --fractional_length)
                base_power *= base;
            arg += static_cast<Float>(fractional_part) / base_power;
        }
    } else {
        in.unget();
    }
    if (negative) arg = -arg;
    return true;
}

/* Strings and character skip */

template <class Device, class T>
inline bool read(binary_io_tag, Device& in, std::string& arg) {
    uint32_t size;
    if (!read(in, size)) return false;
    arg.resize(size);
    return read(in, arg.begin(), arg.end());
}

template <class Device, class T>
inline bool read(text_io_tag, Device& in, const char& arg) {
    detail::skip_characters(in, [arg](char c) { return arg != c; });
    in.unget();
    return true;
}

template <class Device, int N, class... Ts>
inline bool read(text_io_tag, Device& in, char (&arg)[N], Ts&&... args) {
    return read(in, make_range(static_cast<char*>(arg), N), std::forward<Ts>(args)...);
}

template <class Device, class Terminator, class... Ts>
inline bool read(text_io_tag, Device& in, std::string& arg, Terminator&& terminator, Ts&&... args) {
    detail::skip_characters(in, terminator);
    in.unget();
    uint32_t length = std::max(16u, static_cast<uint32_t>(arg.capacity()));
    for (uint32_t last = 0;; length = last += length) {
        arg.resize(last + length);
        uint32_t chars_read =
            static_cast<uint32_t>(detail::read_string(in, &arg[last], length + 1, terminator));
        if (chars_read < length) {
            arg.resize(last + chars_read);
            return read(in, std::forward<Ts>(args)...);
        }
    }
}

template <class Device>
inline bool read(text_io_tag, Device& in, std::string& arg) {
    return read(in, arg, "");
}

/* Ranges */

template <class Device, class T, class... Ts>
inline bool read(io_tag, Device& in, const iterator_range<T>& arg, Ts&&... args) {
    for (auto& element : arg) {
        if (!read(in, element)) return false;
    }
    return read(in, std::forward<Ts>(args)...);
};

template <class Device, class Terminator, class... Ts>
inline bool read(text_io_tag, Device& in, const iterator_range<char*>& arg, Terminator&& terminator,
                 Ts&&... args) {
    detail::skip_characters(in, terminator);
    in.unget();
    detail::read_string(in, arg.begin(), arg.size(), terminator);
    return read(in, std::forward<Ts>(args)...);
}

template <class Device>
inline bool read(text_io_tag, Device& in, const iterator_range<char*>& arg) {
    return read(in, arg, "");
}

/* General forwarding */

template <class Device, class T0, class T1, class... Ts>
inline std::enable_if_t<is_non_const_iterator_v<T0>, bool> read(io_tag, Device& in, T0&& arg0,
                                                                T1&& arg1, Ts&&... args) {
    return read(in, make_range(std::forward<T0>(arg0), std::forward<T1>(arg1)),
                std::forward<Ts>(args)...);
}

template <class Device, class T0, class T1, class... Ts>
inline std::enable_if_t<!is_non_const_iterator_v<T0>, bool> read(io_tag, Device& in, T0&& arg0,
                                                                 T1&& arg1, Ts&&... args) {
    return read(in, std::forward<T0>(arg0)) &&
           read(in, std::forward<T1>(arg1), std::forward<Ts>(args)...);
}

template <class Device, class... Ts>
inline bool read(Device& in, Ts&&... args) {
    static_assert(!std::is_base_of_v<io_tag, Device>, "no matching call for read");
    return read(typename Device::category{}, in, std::forward<Ts>(args)...);
}

template <class Device, class... Ts>
inline bool readln(Device& in, Ts&&... args) {
    static_assert(io_traits<Device>::text, "text device expected");
    return read(in, std::forward<Ts>(args)..., '\n');
}

}  // namespace ac
