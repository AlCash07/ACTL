// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/graph/selectors.hpp>
#include <actl/graph/traits.hpp>
#include <actl/iterator/interface/forward_iterator_interface.hpp>

namespace ac::detail {

template <class E>
struct edge_inverter
{
    E operator()(E e) const
    {
        return E{e.target(), e.source(), e.bundle()};
    }
};

template <class G, class Iter>
class adj_list_out_edge_iter
    : public forward_iterator_interface<adj_list_out_edge_iter<G, Iter>>
{
    G const* g_;
    vertex_t<G> u_;
    Iter iter_;

public:
    using value_type = edge_t<G>;
    using difference_type = std::ptrdiff_t;

    explicit adj_list_out_edge_iter() = default;

    explicit adj_list_out_edge_iter(G const* g, vertex_t<G> u, Iter iter)
        : g_{g}, u_{u}, iter_{iter}
    {}

    Iter id() const
    {
        return iter_;
    }

    edge_t<G> operator*() const
    {
        return g_->get_edge(u_, *iter_);
    }

    adj_list_out_edge_iter& operator++()
    {
        ++iter_;
        return *this;
    }

    friend bool operator==(
        adj_list_out_edge_iter const& lhs, adj_list_out_edge_iter const& rhs)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(lhs.iter_ == rhs.iter_)
};

template <class G>
class adj_list_edge_iter
    : public forward_iterator_interface<adj_list_edge_iter<G>>
{
    bool is_end() const
    {
        return u_ == id_end(g_->vertices_);
    }

    bool is_reverse_edge() const
    {
        if constexpr (G::is_undirected)
            return (*(*this)).target() < u_;
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

    G const* g_;
    vertex_t<G> u_;
    typename G::out_iter iter_;

public:
    using value_type = edge_t<G>;
    using difference_type = std::ptrdiff_t;

    explicit adj_list_edge_iter() = default;

    explicit adj_list_edge_iter(G const* g, bool begin) : g_{g}
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

    edge_t<G> operator*() const
    {
        return g_->get_edge(u_, *iter_);
    }

    adj_list_edge_iter& operator++()
    {
        if (is_end())
            return *this;
        ++iter_;
        skip_empty();
        return *this;
    }

    friend bool operator==(
        adj_list_edge_iter const& lhs, adj_list_edge_iter const& rhs)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(
            lhs.u_ == rhs.u_ && (lhs.is_end() || lhs.iter_ == rhs.iter_))
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
