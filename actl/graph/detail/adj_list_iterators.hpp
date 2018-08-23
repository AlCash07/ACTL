/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/selectors.hpp>
#include <actl/iterator/iterator_facade.hpp>

namespace ac::detail {

template <class It>
class reverse_edge_it : public iterator_adaptor<reverse_edge_it<It>, It> {
public:
    explicit reverse_edge_it(It it) : iterator_adaptor<reverse_edge_it<It>, It>(it) {}

private:
    friend struct ac::iterator_core_access;

    auto dereference() const {
        auto edge = *this->base();
        return typename It::value_type(edge.target(), edge.source(), edge);
    }
};

template <class AdjList, class It, class E = typename AdjList::edge>
class adj_list_out_edge_it
    : public iterator_facade<adj_list_out_edge_it<AdjList, It, E>, std::input_iterator_tag, E, E> {
    friend AdjList;
    friend struct ac::iterator_core_access;

    adj_list_out_edge_it(const AdjList* al, typename AdjList::vertex u, It it)
        : al_(al), u_(u), it_(it) {}

    E dereference() const { return al_->get_edge(u_, *it_); }

    void increment() { ++it_; }

    bool equals(const adj_list_out_edge_it& other) const { return it_ == other.it_; }

    const AdjList*           al_;
    typename AdjList::vertex u_;
    It                       it_;
};

template <class AdjList, class E = typename AdjList::edge>
class adj_list_edge_it
    : public iterator_facade<adj_list_edge_it<AdjList, E>, std::input_iterator_tag, E, E> {
    friend AdjList;
    friend struct ac::iterator_core_access;

    adj_list_edge_it(const AdjList* al, bool begin) : al_(al) {
        if (begin) {
            u_  = al_->vertices_.begin_id();
            it_ = al_->out_begin(u_);
            skip_empty();
        } else {
            u_ = al_->vertices_.end_id();
        }
    }

    E dereference() const { return al_->get_edge(u_, *it_); }

    bool is_end() const { return u_ == al_->vertices_.end_id(); }

    void skip_empty() {
        while (!is_end() && it_ == al_->out_end(u_)) {
            ++u_;
            it_ = al_->out_begin(u_);
        }
    }

    bool is_reverse_edge() const {
        if constexpr (AdjList::is_undirected) {
            return dereference().target() < u_;
        } else {
            return false;
        }
    }

    void increment() {
        if (is_end()) return;
        do {
            ++it_;
            skip_empty();
        } while (!is_end() && is_reverse_edge());
    }

    bool equals(const adj_list_edge_it& other) const {
        return u_ == other.u_ && (is_end() || it_ == other.it_);
    }

    const AdjList*           al_;
    typename AdjList::vertex u_;
    typename AdjList::out_it it_;
};

template <class AL, class S = typename AL::edge_selector>
struct edge_it {
    using type = adj_list_edge_it<AL>;
};

template <class AL>
struct edge_it<AL, two_vertices> {
    using type = typename AL::traits::edges::edge_iterator;
};

}  // namespace ac::detail
