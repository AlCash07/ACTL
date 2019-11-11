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

namespace ac::io {

struct pretty {
    struct format_tag;
};

namespace detail {

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

template <class Char>
struct escaped_string {
    std::basic_string_view<Char> value;
};

template <class Device, class Format, class Char>
inline index write_final(Device& od, Format&, const escaped_string<Char>& s) {
    index res{};
    for (auto c : s.value) {
        auto e = escaped(c);
        res += e ? od.write('\\') + od.write(e) : od.write(c);
    }
    return res;
}

template <class It>
struct map_range {
    iterator_range<It> value;
};

template <class It>
map_range(iterator_range<It>)->map_range<It>;

template <class Device, class Format, class It>
inline index write_final(Device& od, Format& fmt, map_range<It> r) {
    index res{};
    for (const auto& [key, value] : r.value) {
        res += write(od, fmt, key, raw{':'}, value);
    }
    return res;
}

template <class AC>
inline auto make_map_range(const AC& cont) {
    auto res = make_range(cont);
    if constexpr (is_simple_associative_container_v<AC>) {
        return res;
    } else {
        return map_range{res};
    }
}

}  // namespace detail

template <class C, enable_int_if<std::is_same_v<C, char>> = 0>
inline auto serialize(pretty, const C& c) {
    return tuple{raw{'\''}, detail::escaped_string<char>{{&c, 1}}, raw{'\''}};
}

template <class S, enable_int_if<is_string_v<S>> = 0>
inline auto serialize(pretty, const S& s) {
    return tuple{raw{'\"'}, detail::escaped_string<char>{s}, raw{'\"'}};
}

template <class AC, enable_int_if<is_associative_container_v<AC>> = 0>
inline auto serialize(pretty, const AC& cont) {
    return tuple{raw{'{'}, detail::make_map_range(cont), raw{'}'}};
}

template <class SC, enable_int_if<is_sequence_container_v<SC>> = 0>
inline auto serialize(pretty, const SC& cont) {
    return tuple{raw{'['}, make_range(cont), raw{']'}};
}

template <class T, enable_int_if<is_tuple<T>::value> = 0>
inline auto serialize(pretty, const T& x) {
    return tuple{raw{'('}, x, raw{')'}};
}

}  // namespace ac::io
