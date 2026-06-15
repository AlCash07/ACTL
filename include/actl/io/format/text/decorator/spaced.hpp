// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/batch.hpp>
#include <actl/meta/type_traits.hpp>
#include <actl/range/span.hpp>
#include <string_view>

namespace ac::io {

/// Format that inserts delimiter between consecutive output units.
template<typename Space, typename Colon = Space>
struct spaced {
    Space space;
    Colon colon;
    bool separate = false;

    explicit constexpr spaced(Space space, Colon const& colon)
        : space{std::move(space)}, colon{colon} {}

    explicit constexpr spaced(Space space, Colon&& colon)
        : space{std::move(space)}, colon{std::move(colon)} {}

    template<typename S = Space>
        requires are_same_v<S, Colon>
    explicit constexpr spaced(Space const& space) : spaced{space, space} {}

    template<typename S = Space>
        requires are_same_v<char, S, Colon>
    explicit constexpr spaced() : spaced{' '} {}

    struct format_tag;
};

spaced() -> spaced<char>;

template<typename Space, typename Char, size_t N>
spaced(Space, Char const (&)[N]) -> spaced<Space, std::basic_string_view<Char>>;

template<typename T>
auto as_cspan(T const& t) {
    if constexpr (ContiguousRange<T>) {
        return span<range_value_t<T> const>{t};
    } else {
        return span{&t, 1};
    }
}

template<typename S, typename C, typename T>
auto encode(spaced<S, C>& fmt, T& t) {
    using Res = batch<decltype(as_cspan(fmt.space)), T&>;
    if (fmt.separate) {
        return Res{as_cspan(fmt.space), t};
    } else {
        fmt.separate = true;
        return Res{{}, t};
    }
}

template<typename S, typename C, typename T>
decltype(auto) encode(spaced<S, C>& fmt, raw<T> const& t) {
    fmt.separate = false;
    return t;
}

template<typename S, typename C>
auto encode(spaced<S, C>& fmt, colon) {
    fmt.separate = false;
    return batch{colon{}, fmt.colon};
}

template<typename S, typename C, bool Deeper>
void manipulate(spaced<S, C>& fmt, change_level<Deeper>) {
    fmt.separate = !Deeper;
}

} // namespace ac::io
