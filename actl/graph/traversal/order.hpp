/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/events.hpp>
#include <actl/traits/output_type.hpp>

namespace ac {

// Because the graph isn't always a binary tree, the vertex is being output not only between each
// pair of out edges but also before and after them.
template <class VertexOutputIterator>
struct inorder_printer {
    VertexOutputIterator it;

    template <class E>
    void operator()(on_tree_edge, E e) { *it++ = e.source(); }

    void operator()(on_vertex_finish, output_type_t<VertexOutputIterator> u) { *it++ = u; }
};

template <class VertexOutputIterator>
struct postorder_printer {
    VertexOutputIterator it;

    void operator()(on_vertex_finish, output_type_t<VertexOutputIterator> u) { *it++ = u; }
};

template <class VertexOutputIterator>
struct preorder_printer {
    VertexOutputIterator it;

    void operator()(on_vertex_examine, output_type_t<VertexOutputIterator> u) { *it++ = u; }
};

template <class VertexOutputIterator>
inline inorder_printer<VertexOutputIterator> inorder(VertexOutputIterator it) { return {it}; }

template <class VertexOutputIterator>
inline preorder_printer<VertexOutputIterator> preorder(VertexOutputIterator it) { return {it}; }

template <class VertexOutputIterator>
inline postorder_printer<VertexOutputIterator> postorder(VertexOutputIterator it) { return {it}; }

}
