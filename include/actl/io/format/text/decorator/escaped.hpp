// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/batch.hpp>
#include <string_view>

namespace ac::io {

struct escaped {
    struct format_tag;
};

namespace detail {

inline char escape(char c) {
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

template<typename Char>
struct escaped_string {
    std::basic_string_view<Char> value;
};

template<Device Dev, typename Char>
bool write_final(Dev& od, Format auto&, escaped_string<Char> const& s) {
    for (auto c : s.value) {
        using C = char_t<Dev>;
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

auto encode(escaped, std::same_as<char> auto const& c) {
    return batch{'\'', detail::escaped_string<char>{{&c, 1}}, '\''};
}

template<typename S>
    requires is_string_v<S>
auto encode(escaped, S const& s) {
    return batch{'\"', detail::escaped_string<char>{s}, '\"'};
}

} // namespace ac::io
