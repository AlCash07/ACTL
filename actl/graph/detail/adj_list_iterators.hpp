/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/iterator_facade.hpp>
#include <actl/type/none.hpp>

namespace ac::detail {

template <class AdjList, class EId = typename AdjList::edge_id>
class adj_list_out_edge_it : public iterator_facade<adj_list_out_edge_it<AdjList, EId>,
                                                    std::input_iterator_tag, EId, EId> {
    friend AdjList;
    friend struct ac::iterator_core_access;

    adj_list_out_edge_it(typename AdjList::vertex_id u, typename AdjList::out_it it)
        : u_(u), it_(it) {}

    EId dereference() const { return AdjList::get_edge_id(u_, *it_); }

    void increment() { ++it_; }

    bool equals(const adj_list_out_edge_it& other) const { return it_ == other.it_; }

    typename AdjList::vertex_id u_;
    typename AdjList::out_it    it_;
};

template <class AdjList, class EId = typename AdjList::edge_id>
class adj_list_edge_it
    : public iterator_facade<adj_list_edge_it<AdjList, EId>, std::input_iterator_tag, EId, EId> {
    friend AdjList;
    friend struct ac::iterator_core_access;

    adj_list_edge_it(const AdjList* al, bool begin) : al_(al) {
        if (begin) {
            u_  = al_->vertices_.id_begin();
            it_ = al_->out_begin(*u_);
            skip_empty();
        } else {
            u_ = al_->vertices_.id_end();
        }
    }

    EId dereference() const { return AdjList::get_edge_id(*u_, *it_); }

    void skip_empty() {
        while (u_ != al_->vertices_.id_end() && it_ == al_->out_end(*u_)) {
            ++u_;
            it_ = al_->out_begin(*u_);
        }
    }

    void increment() {
        if (u_ != al_->vertices_.id_end()) {
            ++it_;
            skip_empty();
        }
    }

    bool equals(const adj_list_edge_it& other) const {
        return u_ == other.u_ && (u_ == al_->vertices_.id_end() || it_ == other.it_);
    }

    const AdjList*                    al_;
    typename AdjList::vertex_iterator u_;
    typename AdjList::out_it          it_;
};

}  // namespace ac::detail
