// Generalized container functions.
//
// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/traits.hpp>
#include <actl/range/traits/properties.hpp>
#include <algorithm>

namespace ac {

template <class C, class... Ts>
std::pair<range_iterator_t<C>, bool> emplace(C& cont, Ts&&... args)
{
    if constexpr (is_associative_range_v<C>)
    {
        auto res = cont.emplace(std::forward<Ts>(args)...);
        if constexpr (is_unique_range_v<C>)
            return res;
        else
            return {res, true};
    }
    else if constexpr (is_random_access_range_v<C>)
    {
        cont.emplace_back(std::forward<Ts>(args)...);
        return {cont.end() - 1, true};
    }
    else
    {
        return {cont.emplace(cont.end(), std::forward<Ts>(args)...), true};
    }
}

template <class C, class T>
void erase(C& cont, T const& value)
{
    if constexpr (is_associative_range_v<C>)
        cont.erase(value);
    else
        cont.erase(std::remove(cont.begin(), cont.end(), value), cont.end());
}

template <class C, class T>
range_iterator_t<C> find(C& cont, T const& value)
{
    if constexpr (is_associative_range_v<C> && is_sorted_range_v<C>)
    {
        // TODO: make this case work for hash containers.
        // This requires C++20 with find for transparently comparable key.
        return cont.find(value);
    }
    else
    {
        return std::find(ranges::begin(cont), ranges::end(cont), value);
    }
}

} // namespace ac
