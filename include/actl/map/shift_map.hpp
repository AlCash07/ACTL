// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/traits.hpp>
#include <actl/utility/use_default.hpp>

namespace ac {

/// Property map that shifts key domain by the given offset (with casting).
template <class Key, class Value = Key>
class shift_map
{
public:
    explicit constexpr shift_map(Key offset) : offset_{offset} {}

    constexpr Value get(Key key) const
    {
        return static_cast<Value>(key - offset_);
    }

    constexpr Key invert(Value value) const
    {
        return static_cast<Key>(value) + offset_;
    }

private:
    const Key offset_;
};

template <class Value = use_default, class Key>
auto make_shift_map(Key offset)
{
    return shift_map<Key, deduce_t<Value, Key>>(offset);
}

template <class K, class V>
struct const_map_traits<shift_map<K, V>>
    : map_traits_base<K, V, V, true, false, true>
{};

/// Shift property map with offset known at compile-time.
template <auto Offset, class Value = decltype(Offset)>
class static_shift_map
{
    using Key = decltype(Offset);

public:
    constexpr static Value get(Key key)
    {
        return static_cast<Value>(key - Offset);
    }

    constexpr static Key invert(Value value)
    {
        return static_cast<Key>(value) + Offset;
    }
};

template <auto O, class V>
struct const_map_traits<static_shift_map<O, V>>
    : map_traits_base<decltype(O), V, use_default, true, false, true>
{};

} // namespace ac
