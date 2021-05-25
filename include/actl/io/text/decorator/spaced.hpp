// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/batch.hpp>
#include <actl/range/span.hpp>

namespace ac::io {

/// Format that inserts delimiter between consecutive output units.
template <class Space, class Colon = Space>
struct spaced
{
    Space space;
    Colon colon;
    bool separate = false;

    explicit constexpr spaced(Space space, Colon colon)
        : space{std::move(space)}, colon{std::move(colon)}
    {}

    template <class S = Space, enable_int_if<are_same_v<S, Colon>> = 0>
    explicit constexpr spaced(const Space& space) : spaced{space, space}
    {}

    template <class S = Space, enable_int_if<are_same_v<char, S, Colon>> = 0>
    explicit constexpr spaced() : spaced{' '}
    {}

    struct format_tag;
};

spaced()->spaced<char>;

template <class T>
auto as_cspan(const T& x)
{
    if constexpr (is_contiguous_range_v<T>)
    {
        return span<const value_type_t<T>>{x};
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
decltype(auto) encode(spaced<S, C>& fmt, const raw<T>& x)
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
