// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/graph/selectors.hpp>
#include <actl/graph/traits.hpp>
#include <actl/iterator/facade/iterator_facade.hpp>

namespace ac::detail {

template <class E>
struct edge_inverter
{
    E operator()(E e) const
    {
        return E{e.target(), e.source(), e.bundle()};
    }
};

template <class G>
struct edge_iter_types
{
    using iterator_category = std::input_iterator_tag;
    using value_type = edge_t<G>;
    using reference = edge_t<G>;
    using difference_type = use_default;
};

template <class G, class Iter>
class adj_list_out_edge_iter
    : public iterator_facade<
          adj_list_out_edge_iter<G, Iter>,
          edge_iter_types<G>>
{
    friend struct ac::iterator_core_access;

    edge_t<G> dereference() const
    {
        return g_->get_edge(u_, *iter_);
    }

    void increment()
    {
        ++iter_;
    }

    bool equals(const adj_list_out_edge_iter& rhs) const
    {
        return iter_ == rhs.iter_;
    }

    const G* g_;
    vertex_t<G> u_;
    Iter iter_;

public:
    explicit adj_list_out_edge_iter() = default;

    explicit adj_list_out_edge_iter(const G* g, vertex_t<G> u, Iter iter)
        : g_{g}, u_{u}, iter_{iter}
    {}

    Iter id() const
    {
        return iter_;
    }
};

template <class G>
class adj_list_edge_iter
    : public iterator_facade<adj_list_edge_iter<G>, edge_iter_types<G>>
{
    friend struct ac::iterator_core_access;

    edge_t<G> dereference() const
    {
        return g_->get_edge(u_, *iter_);
    }

    bool is_end() const
    {
        return u_ == id_end(g_->vertices_);
    }

    bool is_reverse_edge() const
    {
        if constexpr (G::is_undirected)
            return dereference().target() < u_;
        else
            return false;
    }

    void skip_empty()
    {
        while (!is_end())
        {
            if (iter_ == g_->out_end(u_))
            {
                ++u_;
                if (!is_end())
                    iter_ = g_->out_begin(u_);
            }
            else
            {
                if (is_reverse_edge())
                    ++iter_;
                else
                    break;
            }
        }
    }

    void increment()
    {
        if (is_end())
            return;
        ++iter_;
        skip_empty();
    }

    bool equals(const adj_list_edge_iter& rhs) const
    {
        return u_ == rhs.u_ && (is_end() || iter_ == rhs.iter_);
    }

    const G* g_;
    vertex_t<G> u_;
    typename G::out_iter iter_;

public:
    explicit adj_list_edge_iter() = default;

    explicit adj_list_edge_iter(const G* g, bool begin) : g_{g}
    {
        if (begin)
        {
            u_ = id_begin(g_->vertices_);
            iter_ = g_->out_begin(u_);
            skip_empty();
        }
        else
        {
            u_ = id_end(g_->vertices_);
        }
    }
};

template <class G, class = typename G::edge_selector>
struct edge_iter
{
    using type = adj_list_edge_iter<G>;
};

template <class G>
struct edge_iter<G, two_vertices>
{
    using type = typename G::traits::edges::template edge_iterator<edge_t<G>>;
};

} // namespace ac::detail
