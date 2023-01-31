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
template <class Space, class Colon = Space>
struct spaced
{
    Space space;
    Colon colon;
    bool separate = false;

    explicit constexpr spaced(Space space, Colon const& colon)
        : space{std::move(space)}, colon{colon}
    {}

    explicit constexpr spaced(Space space, Colon&& colon)
        : space{std::move(space)}, colon{std::move(colon)}
    {}

    template <class S = Space>
        requires are_same_v<S, Colon>
    explicit constexpr spaced(Space const& space) : spaced{space, space}
    {}

    template <class S = Space>
        requires are_same_v<char, S, Colon>
    explicit constexpr spaced() : spaced{' '}
    {}

    struct format_tag;
};

spaced()->spaced<char>;

template <class Space, class Char, size_t N>
spaced(Space, Char const (&)[N]) -> spaced<Space, std::basic_string_view<Char>>;

template <class T>
auto as_cspan(T const& x)
{
    if constexpr (ContiguousRange<T>)
    {
        return span<range_value_t<T> const>{x};
    }
    else
    {
        return span{&x, 1};
    }
}

template <class S, class C, class T>
auto encode(spaced<S, C>& fmt, T& x)
{
    using Res = batch<decltype(as_cspan(fmt.space)), T&>;
    if (fmt.separate)
    {
        return Res{as_cspan(fmt.space), x};
    }
    else
    {
        fmt.separate = true;
        return Res{{}, x};
    }
}

template <class S, class C, class T>
decltype(auto) encode(spaced<S, C>& fmt, raw<T> const& x)
{
    fmt.separate = false;
    return x;
}

template <class S, class C>
auto encode(spaced<S, C>& fmt, colon)
{
    fmt.separate = false;
    return batch{colon{}, fmt.colon};
}

template <class S, class C, bool Deeper>
void manipulate(spaced<S, C>& fmt, change_level<Deeper>)
{
    fmt.separate = !Deeper;
}

} // namespace ac::io
