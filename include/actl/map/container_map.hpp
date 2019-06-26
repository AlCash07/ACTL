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
#include <actl/range/algorithm.hpp>
#include <actl/range/container_range.hpp>

namespace ac {

template <class Container>
class container_map : public put_helper<container_map<Container>>,
                      public container_range<Container> {
    using base_t = container_range<Container>;
    using base_t::data_;

    using C = remove_cvref_t<Container>;
    using It = typename base_t::iterator;

public:
    static constexpr bool invertible = false;
    static constexpr bool iterable = true;

    using key_type = container_id<C>;
    using reference =
        std::conditional_t<base_t::writable, typename C::reference, typename C::const_reference>;
    using key_ref = std::pair<key_type, reference>;

    class iterator
        : public iterator_adaptor<iterator, It, use_default, key_ref, key_ref, const key_ref*> {
        iterator(It it, const Container& cont)
            : iterator_adaptor<iterator, It, use_default, key_ref, key_ref, const key_ref*>{it}
            , cont_{cont} {}

        key_ref dereference() const { return {iterator_to_id(cont_, this->base()), *this->base()}; }

        const Container& cont_;

        friend class container_map;
        friend struct ac::iterator_core_access;
    };

    using base_t::base_t;

    friend reference get(const container_map& map, key_type key) { return id_at(map.data_, key); }

    template <bool W = base_t::writable, class = std::enable_if_t<W>>
    void clear() {
        fill(data_, typename base_t::value_type{});
    }

    iterator begin() const { return {data_.begin(), data_}; }
    iterator end() const { return {data_.end(), data_}; }
};

template <class C>
container_map(C&&) -> container_map<remove_rvalue_ref_t<C>>;

}  // namespace ac
