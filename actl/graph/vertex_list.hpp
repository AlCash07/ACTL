/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/selectors.hpp>
#include <actl/property_map/generic_container_property_map.hpp>

namespace ac {

template <class VertexContainer = none, class T = value_type_t<VertexContainer>>
class vertex_list : public vertex_list<VertexContainer, none> {
    using base_t = vertex_list<VertexContainer, none>;

public:
    using vertex_id = typename base_t::vertex_id;

    using base_t::base_t;

    auto operator[](vertex_property) {
        return make_generic_container_property_map(this->vertices_);
    }

    auto operator[](vertex_property) const {
        return make_generic_container_property_map(this->vertices_);
    }

    T&       operator[](vertex_id u)       { return get((*this)[vertex_property{}], u); }
    const T& operator[](vertex_id u) const { return get((*this)[vertex_property{}], u); }
};

template <class VC>
class vertex_list<VC, none> {
    using vertex_container = generic_container<VC>;

public:
    using vertex_id       = typename vertex_container::id;
    using vertex_iterator = typename vertex_container::id_iterator;

    explicit vertex_list() = default;

    explicit vertex_list(int n) : vertices_(n) {}

    int vertex_count() const { return vertices_.size(); }

    range<vertex_iterator> vertices() const { return vertices_.id_range(); }

    vertex_id nth_vertex(int n) const {
        ACTL_ASSERT(0 <= n && n < vertex_count());
        return *std::next(vertices_.id_range().begin(), n);
    }

    template <class... Ts>
    std::pair<vertex_id, bool> try_add_vertex(Ts&&... args) {
        return vertices_.emplace(std::forward<Ts>(args)...);
    }

    template <class... Ts>
    vertex_id add_vertex(Ts&&... args) {
        return try_add_vertex(std::forward<Ts>(args)...).first;
    }

    void remove_vertex(vertex_id u) { vertices_.erase(u); }

    void clear() { vertices_.clear(); }

    void swap(vertex_list& other) { vertices_.swap(other.vertices_); }

    none operator[](vertex_id)       { return none{}; }
    none operator[](vertex_id) const { return none{}; }

protected:
    vertex_container vertices_;
};

}  // namespace ac
