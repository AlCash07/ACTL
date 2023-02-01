// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/map/traits.hpp>
#include <actl/range/iterator/transform_iterator.hpp>
#include <actl/range/iterator_range.hpp>
#include <actl/range/traits/properties.hpp>

namespace ac {

namespace detail {

template <class C>
struct get_id_ref
{
    // Pointer is used instead of a reference to support copy assignment
    // required for std::copyable.
    C* cont;

    map_pair_t<C> operator()(container_id<C> id) const
    {
        return {id, id_at(*cont, id)};
    }
};

} // namespace detail

template <Container C>
    requires(!PairAssociativeRange<C>)
struct map_traits<C>
    : map_traits_base<
          container_id<C>,
          reference_t<C>,
          range_value_t<C>,
          true,
          !std::is_const_v<C>,
          false,
          true,
          iterator_range<transform_iterator<
              container_id_iterator<C>,
              detail::get_id_ref<C>>>>
{};

template <Container C>
    requires(!PairAssociativeRange<C>)
struct map_ops<C> : map_put<C>
{
    static map_reference_t<C> get(C& map, map_key_t<C> key)
    {
        return id_at(map, key);
    }

    static map_range_t<C> map_range(C& map)
    {
        auto r = id_range(map);
        return {{r.begin(), &map}, {r.end(), &map}};
    }
};

} // namespace ac
