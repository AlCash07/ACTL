// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

namespace ac {

template<typename Graph>
using vertex_t = typename Graph::vertex;

template<typename Graph>
using edge_t = typename Graph::edge;

template<typename Graph>
using out_edge_t = typename Graph::out_edge;

template<typename Graph>
using in_edge_t = typename Graph::in_edge;

template<typename Graph>
using out_edge_iterator_t = typename Graph::out_edge_iterator;

template<typename Graph>
using in_edge_iterator_t = typename Graph::in_edge_iterator;

} // namespace ac
