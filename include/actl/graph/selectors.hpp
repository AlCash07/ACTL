// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

namespace ac {

/* Selectors for Directed template parameter */

struct directed {};
struct undirected {};
struct bidirectional {};

/* Selectors for EdgeContainer value type */

struct edge_property {};
struct one_vertex {};
struct two_vertices {};

/* Use graph[vertex_property{}] or graph[edge_property{}] to get graph bundled
 * properties */

struct vertex_property {};

} // namespace ac
