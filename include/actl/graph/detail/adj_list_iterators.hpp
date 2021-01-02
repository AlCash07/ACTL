// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/graph/selectors.hpp>
#include <actl/graph/traits.hpp>
#include <actl/iterator/facade/iterator_facade.hpp>

namespace ac::detail {

template <class E>
struct edge_inverter {
    E operator()(E e) const {
        return E{e.target(), e.source(), e.bundle()};
    }
};

template <class G>
using edge_it_types = iterator_types<std::input_iterator_tag, edge_t<G>, edge_t<G>>;

template <class G, class It>
class adj_list_out_edge_it : public iterator_facade<adj_list_out_edge_it<G, It>, edge_it_types<G>> {
    friend struct ac::iterator_core_access;

    edge_t<G> dereference() const {
        return g_->get_edge(u_, *it_);
    }

    void increment() {
        ++it_;
    }

    bool equals(const adj_list_out_edge_it& rhs) const {
        return it_ == rhs.it_;
    }

    const G* g_;
    vertex_t<G> u_;
    It it_;

public:
    explicit adj_list_out_edge_it() = default;

    explicit adj_list_out_edge_it(const G* g, vertex_t<G> u, It it) : g_{g}, u_{u}, it_{it} {}

    It id() const {
        return it_;
    }
};

template <class G>
class adj_list_edge_it : public iterator_facade<adj_list_edge_it<G>, edge_it_types<G>> {
    friend struct ac::iterator_core_access;

    edge_t<G> dereference() const {
        return g_->get_edge(u_, *it_);
    }

    bool is_end() const {
        return u_ == id_end(g_->vertices_);
    }

    bool is_reverse_edge() const {
        if constexpr (G::is_undirected) {
            return dereference().target() < u_;
        } else {
            return false;
        }
    }

    void skip_empty() {
        while (!is_end()) {
            if (it_ == g_->out_end(u_)) {
                ++u_;
                if (!is_end())
                    it_ = g_->out_begin(u_);
            } else {
                if (is_reverse_edge()) {
                    ++it_;
                } else {
                    break;
                }
            }
        }
    }

    void increment() {
        if (is_end())
            return;
        ++it_;
        skip_empty();
    }

    bool equals(const adj_list_edge_it& rhs) const {
        return u_ == rhs.u_ && (is_end() || it_ == rhs.it_);
    }

    const G* g_;
    vertex_t<G> u_;
    typename G::out_it it_;

public:
    explicit adj_list_edge_it() = default;

    explicit adj_list_edge_it(const G* g, bool begin) : g_{g} {
        if (begin) {
            u_ = id_begin(g_->vertices_);
            it_ = g_->out_begin(u_);
            skip_empty();
        } else {
            u_ = id_end(g_->vertices_);
        }
    }
};

template <class G, class = typename G::edge_selector>
struct edge_it {
    using type = adj_list_edge_it<G>;
};

template <class G>
struct edge_it<G, two_vertices> {
    using type = typename G::traits::edges::template edge_iterator<edge_t<G>>;
};

}  // namespace ac::detail
