// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/container_id.hpp>
#include <actl/graph/selectors.hpp>
#include <actl/graph/traits.hpp>
#include <actl/range/iterator/interface/forward_iterator_interface.hpp>

namespace ac::detail {

template<typename E>
struct edge_inverter {
    E operator()(E e) const {
        return E{e.target(), e.source(), e.bundle()};
    }
};

template<typename G, typename Iter>
class adj_list_out_edge_iter
    : public forward_iterator_interface<adj_list_out_edge_iter<G, Iter>> {
    G const* m_g;
    vertex_t<G> m_u;
    Iter m_iter;

public:
    using value_type = edge_t<G>;
    using difference_type = std::ptrdiff_t;

    explicit adj_list_out_edge_iter() = default;

    explicit adj_list_out_edge_iter(G const* g, vertex_t<G> u, Iter iter)
        : m_g{g}, m_u{u}, m_iter{iter} {}

    Iter id() const {
        return m_iter;
    }

    edge_t<G> operator*() const {
        return m_g->get_edge(m_u, *m_iter);
    }

    adj_list_out_edge_iter& operator++() {
        ++m_iter;
        return *this;
    }

    friend bool operator==(
        adj_list_out_edge_iter const& l, adj_list_out_edge_iter const& r
    ) AC_DEDUCE_NOEXCEPT_AND_RETURN(l.m_iter == r.m_iter)
};

template<typename G>
class adj_list_edge_iter
    : public forward_iterator_interface<adj_list_edge_iter<G>> {
    bool is_end() const {
        return m_u == id_end(m_g->m_vertices);
    }

    bool is_reverse_edge() const {
        if constexpr (G::is_undirected)
            return (*(*this)).target() < m_u;
        else
            return false;
    }

    void skip_empty() {
        while (!is_end()) {
            if (m_iter == m_g->out_end(m_u)) {
                ++m_u;
                if (!is_end())
                    m_iter = m_g->out_begin(m_u);
            } else {
                if (is_reverse_edge())
                    ++m_iter;
                else
                    break;
            }
        }
    }

    G const* m_g;
    vertex_t<G> m_u;
    typename G::out_iter m_iter;

public:
    using value_type = edge_t<G>;
    using difference_type = std::ptrdiff_t;

    explicit adj_list_edge_iter() = default;

    explicit adj_list_edge_iter(G const* g, bool begin) : m_g{g} {
        if (begin) {
            m_u = id_begin(m_g->m_vertices);
            m_iter = m_g->out_begin(m_u);
            skip_empty();
        } else {
            m_u = id_end(m_g->m_vertices);
        }
    }

    edge_t<G> operator*() const {
        return m_g->get_edge(m_u, *m_iter);
    }

    adj_list_edge_iter& operator++() {
        if (is_end())
            return *this;
        ++m_iter;
        skip_empty();
        return *this;
    }

    friend bool operator==(
        adj_list_edge_iter const& l, adj_list_edge_iter const& r
    )
        AC_DEDUCE_NOEXCEPT_AND_RETURN(
            l.m_u == r.m_u && (l.is_end() || l.m_iter == r.m_iter)
        )
};

template<typename G, typename = typename G::edge_selector>
struct edge_iter {
    using type = adj_list_edge_iter<G>;
};

template<typename G>
struct edge_iter<G, two_vertices> {
    using type = typename G::traits::edges::template edge_iterator<edge_t<G>>;
};

} // namespace ac::detail
