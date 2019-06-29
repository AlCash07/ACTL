/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/map/map.hpp>

namespace ac {

namespace detail {

template <class T>
using enable_int_if_gc =
    std::enable_if_t<is_container_v<T> && !is_multiple_associative_container_v<T> &&
                         !is_pair_associative_container_v<T>,
                     int>;

template <class C>
class container_map_range {
    using It = container_id_iterator<C>;
    using Pair = typename map_traits<C>::pair_type;

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

    C& cont_;

    explicit container_map_range(C& cont) : cont_{cont} {}

    iterator begin() const { return {It{id_begin(cont_)}, cont_}; }
    iterator end() const { return {It{id_end(cont_)}, cont_}; }
};

}  // namespace detail

template <class C>
struct map_types<C, std::void_t<detail::enable_int_if_gc<C>>> {
    using key_type = container_id<C>;
    using reference =
        std::conditional_t<std::is_const_v<C>, typename C::const_reference, typename C::reference>;
};

template <class C, detail::enable_int_if_gc<C> = 0>
inline typename map_traits<C>::reference get(C& map, typename map_traits<C>::key_type key) {
    return id_at(map, key);
}

template <class C, detail::enable_int_if_gc<C> = 0, std::enable_if_t<!std::is_const_v<C>, int> = 0>
inline void put(C& map, typename map_traits<C>::key_type key,
                typename map_traits<C>::value_type value) {
    id_at(map, key) = value;
}

template <class C, detail::enable_int_if_gc<C> = 0>
inline auto map_range(C& map) {
    return detail::container_map_range{map};
}

}  // namespace ac
