/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/edge_list.hpp>
#include <actl/graph/vertex_list.hpp>

namespace ac {

template <class Directed,
          class EdgeContainer   = std::vector<none>,
          class VertexContainer = none>
class vertex_edge_list
    : public vertex_list<VertexContainer>,
      public edge_list<Directed, container_id_t<VertexContainer>, EdgeContainer> {
    using vbase_t = vertex_list<VertexContainer>;
    using ebase_t = edge_list<Directed, container_id_t<VertexContainer>, EdgeContainer>;

public:
    using vertex_id = typename vbase_t::vertex_id;
    using edge_id   = typename ebase_t::edge_id;

    using vbase_t::vbase_t;

    template <class... Ts>
    std::pair<edge_id, bool> try_add_edge(vertex_id u, vertex_id v, Ts&&... args) {
        if constexpr (is_random_access_v<VertexContainer>) {
            vertex_id n = std::max(u, v);
            if (n >= this->vertices_.size()) this->vertices_.resize(n + 1);
        }
        return ebase_t::try_add_edge(u, v, std::forward<Ts>(args)...);
    }

    template <class... Ts>
    edge_id add_edge(vertex_id u, vertex_id v, Ts&&... args) {
        return try_add_edge(u, v, std::forward<Ts>(args)...).first;
    }

    // TODO: implement using one pass over edges that will remove edges and decrease int IDs.
    void remove_vertex(vertex_id u);

    void clear() {
        vbase_t::clear();
        ebase_t::clear();
    }

    void swap(vertex_edge_list& other) {
        vbase_t::swap(other);
        ebase_t::swap(other);
    }
};

}  // namespace ac
