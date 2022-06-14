// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/functions.hpp>
#include <actl/io/argument/byte_span.hpp>
#include <actl/io/argument/size.hpp>
#include <actl/io/core/batch.hpp>
#include <actl/meta/static_size.hpp>
#include <actl/range/traits/category.hpp>
#include <actl/range/traits/properties.hpp>

namespace ac::io {

template <class Pair>
decltype(auto) key_representation(Pair& x)
{
    if constexpr (std::is_const_v<Pair>)
        return x.first;
    else
        // const_cast is used to read std::map<Key, Value>::value_type
        // which is std::pair<Key const, Value>.
        return const_cast<typename Pair::first_type&>(x.first);
}

template <class Range, class T>
decltype(auto) element_representation(T& x)
{
    if constexpr (is_pair_associative_range_v<Range>)
        return batch{key_representation(x), colon{}, x.second};
    else
        return x;
}

template <class Device, class Format, class R, enable_int_if<is_range_v<R>> = 0>
bool write_final(Device& od, Format& fmt, R const& x)
{
    nested_scope_guard g{fmt};
    if constexpr (is_container_v<R> && static_size_v<R> == dynamic_size)
        if (!write(od, fmt, size{x.size()}))
            return false;
    for (auto const& value : x)
        if (!write(od, fmt, element_representation<R>(value)))
            return false;
    return true;
}

template <class D, class F, class R>
bool read_range(D& id, F& fmt, R& x)
{
    for (auto& value : x)
    {
        if (!read(id, fmt, value))
            return false;
    }
    return true;
}

template <class D, class F, class C>
bool read_container(D& id, F& fmt, C& x)
{
    decltype(x.size()) size{};
    if (!read(id, fmt, io::size{size}))
        return false;
    if constexpr (!is_random_access_range_v<C>)
    {
        for (; size > 0; --size)
        {
            range_value_t<C> value;
            if (!read(id, fmt, element_representation<C>(value)))
                return false;
            emplace(x, std::move(value));
        }
        return true;
    }
    else
    {
        x.resize(size);
        return read_range(id, fmt, x);
    }
}

template <
    class Device,
    class Format,
    class R,
    enable_int_if<is_range_v<R> && !std::is_const_v<range_value_t<R>>> = 0>
bool read_final(Device& id, Format& fmt, R& x)
{
    nested_scope_guard g{fmt};
    if constexpr (is_container_v<R> && static_size_v<R> == dynamic_size)
        return read_container(id, fmt, x);
    else
        return read_range(id, fmt, x);
}

} // namespace ac::io
