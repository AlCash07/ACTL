/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/traits.hpp>
#include <actl/io/range.hpp>
#include <actl/io/util/raw.hpp>
#include <actl/range/iterator_range.hpp>
#include <actl/string/traits.hpp>

namespace ac::io {

template <class Tag>
struct pretty_tag {
    using base = Tag;
};

template <class Format>
struct pretty : Format {
    using format_tag = pretty_tag<typename Format::format_tag>;
};

namespace detail {

template <class T, class C>
struct is_span : std::false_type {};

template <index N, class C>
struct is_span<span<C, N>, C> : std::true_type {};

}  // namespace detail

inline char escaped(char c) {
    switch (c) {
        case '\0': return '0';
        case '\a': return 'a';
        case '\b': return 'b';
        case '\t': return 't';
        case '\n': return 'n';
        case '\v': return 'v';
        case '\f': return 'f';
        case '\r': return 'r';
        case '\"': return '"';
        case '\'': return '\'';
        case '\\': return '\\';
        default: return char{};
    }
}

template <class Device, class Format, class T, class Tag>
inline index serialize(Device& od, Format& fmt, const T& x, pretty_tag<Tag>) {
    using C = char_t<Device>;
    auto write_raw = [&](C c) { return serialize(od, fmt, raw{c}, Tag{}); };
    auto write_escaped = [&](C c) {
        auto e = escaped(c);
        return e ? od.write('\\') + od.write(e) : od.write(c);
    };
    if constexpr (std::is_same_v<T, C>) {
        return write_raw('\'') + write_escaped(x) + write_raw('\'');
    } else if constexpr (is_string_v<T, const C>) {
        index res = write_raw('"');
        for (auto c : char_span{x}) res += write_escaped(c);
        return res + write_raw('"');
    } else if constexpr (is_range_v<T> && !detail::is_span<T, const C>::value) {
        if constexpr (is_associative_container_v<T>) {
            index res = write_raw('{');
            if constexpr (is_simple_associative_container_v<T>) {
                res += serialize(od, fmt, make_range(x), Tag{});
            } else {
                for (const auto& [key, value] : x) {
                    res += write(od, fmt, key, raw{':'}, value);
                }
            }
            return res + write_raw('}');
        } else {
            return write_raw('[') + serialize(od, fmt, make_range(x), Tag{}) + write_raw(']');
        }
    } else if constexpr (is_composite<T>::value) {
        return write_raw('(') + serialize(od, fmt, x, Tag{}) + write_raw(')');
    } else {
        return serialize(od, fmt, x, Tag{});
    }
}

}  // namespace ac::io
