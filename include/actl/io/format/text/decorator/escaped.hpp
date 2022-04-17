// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/batch.hpp>
#include <actl/meta/type_traits.hpp>
#include <string_view>

namespace ac::io {

struct escaped
{
    struct format_tag;
};

namespace detail {

inline char escape(char c)
{
    // clang-format off
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
    // clang-format on
}

template <class Char>
struct escaped_string
{
    std::basic_string_view<Char> value;
};

template <class Device, class Format, class Char>
bool write_final(Device& od, Format&, const escaped_string<Char>& s)
{
    for (auto c : s.value)
    {
        using C = char_t<Device>;
        auto e = escape(c);
        bool ok =
            e ? od.write(static_cast<C>('\\')) && od.write(static_cast<C>(e))
              : od.write(static_cast<C>(c));
        if (!ok)
            return false;
    }
    return true;
}

} // namespace detail

template <class C, enable_int_if<std::is_same_v<C, char>> = 0>
auto encode(escaped, const C& c)
{
    return batch{'\'', detail::escaped_string<char>{{&c, 1}}, '\''};
}

template <class S, enable_int_if<is_string_v<S>> = 0>
auto encode(escaped, const S& s)
{
    return batch{'\"', detail::escaped_string<char>{s}, '\"'};
}

} // namespace ac::io
