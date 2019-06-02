/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/composite.hpp>
#include <actl/io/manip/raw.hpp>
#include <actl/range/container_range.hpp>
#include <actl/traits/container_traits.hpp>
#include <actl/traits/string_traits.hpp>

namespace ac::io {

template <class Format>
class pretty : public Format {};

template <class Tag>
struct pretty_tag {
    using base = Tag;
};

template <class Format>
struct format_traits<pretty<Format>> {
    using tag = pretty_tag<format_tag_t<Format>>;
};

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

template <class T>
struct is_char_span : std::false_type {};

template <class Char>
struct is_char_span<char_span<Char>> : std::true_type {};

template <class Device, class Format, class T, class Tag,
          class = std::enable_if_t<!is_char_span<T>::value>>
inline index serialize(Device& id, Format& fmt, const T& x, pretty_tag<Tag>) {
    using C = char_t<Device>;
    auto write_raw = [&](C c) { return serialize(id, fmt, raw{c}, Tag{}); };
    auto write_escaped = [&](C c) {
        auto e = escaped(c);
        return e ? id.put('\\') + id.put(e) : id.put(c);
    };
    if constexpr (is_range_v<T>) {
        if constexpr (is_string_v<T>) {
            index res = write_raw('"');
            for (auto c : char_span{x}) res += write_escaped(c);
            return res + write_raw('"');
        } else if constexpr (is_associative_container_v<T>) {
            return write_raw('{') + serialize(id, fmt, make_range(x), Tag{}) + write_raw('}');
        } else {
            return write_raw('[') + serialize(id, fmt, make_range(x), Tag{}) + write_raw(']');
        }
    } else if constexpr (std::is_same_v<T, C>) {
        return write_raw('\'') + write_escaped(x) + write_raw('\'');
    } else if constexpr (is_composite<T>::value) {
        return write_raw('(') + serialize(id, fmt, x, Tag{}) + write_raw(')');
    } else {
        return serialize(id, fmt, x, Tag{});
    }
}

}  // namespace ac::io
