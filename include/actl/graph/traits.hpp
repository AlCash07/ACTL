/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

namespace ac {

template <class Graph>
using vertex_t = typename Graph::vertex;

template <class Graph>
using edge_t = typename Graph::edge;

template <class Graph>
using out_edge_t = typename Graph::out_edge;

template <class Graph>
using in_edge_t = typename Graph::in_edge;

}  // namespace ac
