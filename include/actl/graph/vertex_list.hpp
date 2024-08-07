// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/graph/detail/list_value.hpp>
#include <actl/graph/selectors.hpp>
#include <actl/map/generic_container.hpp>

namespace ac {

template<
    class VertexContainer = none,
    class T = graph::list_value_t<VertexContainer>>
class vertex_list : public vertex_list<VertexContainer, none> {
    using base_t = vertex_list<VertexContainer, none>;

public:
    using typename base_t::vertex;

    using base_t::base_t;

    auto operator[](vertex_property) {
        return std::ref(this->vertices_);
    }

    auto operator[](vertex_property) const {
        return std::ref(this->vertices_);
    }

    T& operator[](vertex u) {
        return get(this->vertices_, u);
    }

    T const& operator[](vertex u) const {
        return get(this->vertices_, u);
    }
};

template<class VC>
class vertex_list<VC, none> {
public:
    using vertex_container =
        rebind_container_t<VC, graph::list_value_t<VC>>; // to handle none
    using vertex = container_id<vertex_container>;
    using vertex_iterator = container_id_iterator<vertex_container>;

private:
    static constexpr bool RA = RandomAccessRange<vertex_container>;

public:
    vertex_list() = default;

    template<bool B = RA>
        requires B
    explicit vertex_list(index n) {
        resize(n);
    }

    index vertex_count() const {
        return static_cast<index>(vertices_.size());
    }

    template<bool B = RA>
        requires B
    void resize(index n) {
        vertices_.resize(static_cast<range_size_t<vertex_container>>(n));
    }

    iterator_range<vertex_iterator> vertices() const {
        return id_range(vertices_);
    }

    vertex null_vertex() const {
        return id_null(vertices_);
    }

    vertex nth_vertex(
        std::iter_difference_t<container_id_iterator<vertex_container>> n
    ) const {
        AC_ASSERT(0 <= n && n < vertex_count());
        return *std::next(id_range(vertices_).begin(), n);
    }

    template<class... Ts>
    std::pair<vertex, bool> try_add_vertex(Ts&&... args) {
        return id_emplace(vertices_, std::forward<Ts>(args)...);
    }

    template<class... Ts>
    vertex add_vertex(Ts&&... args) {
        return try_add_vertex(std::forward<Ts>(args)...).first;
    }

    void remove_vertex(vertex u) {
        id_erase(vertices_, u);
    }

    void swap(vertex_list& rhs) {
        vertices_.swap(rhs.vertices_);
    }

    void operator[](vertex) const {}

protected:
    vertex_container vertices_;
};

} // namespace ac
