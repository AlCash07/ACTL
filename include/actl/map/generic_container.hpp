/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/map/traits.hpp>

namespace ac {

namespace detail {

template <class T>
using enable_if_gc_t = std::enable_if_t<is_container_v<T> && !is_pair_associative_container_v<T>>;

template <class C>
class container_map_range {
    using It = container_id_iterator<C>;
    using Pair = map_pair_t<C>;

    C& cont_;

public:
    class iterator : public iterator_adaptor<iterator, It, use_default, Pair, Pair, const Pair*> {
        iterator(It it, C& cont)
            : iterator_adaptor<iterator, It, use_default, Pair, Pair, const Pair*>{it}
            , cont_{cont} {}

        Pair dereference() const {
            auto id = *this->base();
            return {id, id_at(cont_, id)};
        }

        C& cont_;

        friend class container_map_range;
        friend struct ac::iterator_core_access;
    };

    explicit container_map_range(C& cont) : cont_{cont} {}

    iterator begin() const { return {It{id_begin(cont_)}, cont_}; }
    iterator end() const { return {It{id_end(cont_)}, cont_}; }
};

}  // namespace detail

template <class C>
struct map_traits<C, detail::enable_if_gc_t<C>>
    : map_traits_base<container_id<C>, reference_t<C>, value_t<C>, true, !std::is_const_v<C>, false,
                      true, detail::container_map_range<C>> {};

template <class C>
struct map_ops<C, detail::enable_if_gc_t<C>> : map_put<C> {
    static map_reference_t<C> get(C& map, map_key_t<C> key) { return id_at(map, key); }

    static auto map_range(C& map) { return map_range_t<C>{map}; }
};

}  // namespace ac
