/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/graph/selectors.hpp>
#include <actl/iterator/iterator_adaptor.hpp>

namespace ac::detail {

template <class It>
class reverse_edge_it : public iterator_adaptor<reverse_edge_it<It>, It> {
public:
    explicit reverse_edge_it(It it) : iterator_adaptor<reverse_edge_it<It>, It>{it} {}

private:
    friend struct ac::iterator_core_access;

    auto dereference() const {
        auto edge = *this->base();
        return value_t<It>{edge.target(), edge.source(), edge.bundle()};
    }
};

template <class AdjList, class It, class E = typename AdjList::edge>
class adj_list_out_edge_it
    : public iterator_facade<adj_list_out_edge_it<AdjList, It, E>, std::input_iterator_tag, E, E> {
    friend struct ac::iterator_core_access;

    using vertex = typename AdjList::vertex;

    E dereference() const { return al_->get_edge(u_, *it_); }

    void increment() { ++it_; }

    bool equals(const adj_list_out_edge_it& rhs) const { return it_ == rhs.it_; }

    const AdjList* al_;
    vertex u_;
    It it_;

public:
    explicit adj_list_out_edge_it() = default;

    explicit adj_list_out_edge_it(const AdjList* al, vertex u, It it) : al_{al}, u_{u}, it_{it} {}

    It id() const { return it_; }
};

template <class AdjList, class E = typename AdjList::edge>
class adj_list_edge_it
    : public iterator_facade<adj_list_edge_it<AdjList, E>, std::input_iterator_tag, E, E> {
    friend struct ac::iterator_core_access;

    E dereference() const { return al_->get_edge(u_, *it_); }

    bool is_end() const { return u_ == id_end(al_->vertices_); }

    bool is_reverse_edge() const {
        if constexpr (AdjList::is_undirected) {
            return dereference().target() < u_;
        } else {
            return false;
        }
    }

    void skip_empty() {
        while (!is_end()) {
            if (it_ == al_->out_end(u_)) {
                ++u_;
                if (!is_end()) it_ = al_->out_begin(u_);
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
        if (is_end()) return;
        ++it_;
        skip_empty();
    }

    bool equals(const adj_list_edge_it& rhs) const {
        return u_ == rhs.u_ && (is_end() || it_ == rhs.it_);
    }

    const AdjList* al_;
    typename AdjList::vertex u_;
    typename AdjList::out_it it_;

public:
    explicit adj_list_edge_it() = default;

    explicit adj_list_edge_it(const AdjList* al, bool begin) : al_{al} {
        if (begin) {
            u_ = id_begin(al_->vertices_);
            it_ = al_->out_begin(u_);
            skip_empty();
        } else {
            u_ = id_end(al_->vertices_);
        }
    }
};

template <class AL, class S = typename AL::edge_selector>
struct edge_it {
    using type = adj_list_edge_it<AL>;
};

template <class AL>
struct edge_it<AL, two_vertices> {
    using type = typename AL::traits::edges::template edge_iterator<typename AL::edge>;
};

}  // namespace ac::detail
