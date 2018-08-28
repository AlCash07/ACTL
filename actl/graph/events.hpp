/***************************************************************************************************
 * Structures used for tag dispatching in algorithm components.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

namespace ac {

struct on_vertex_initialize {};
struct on_vertex_start {};
struct on_vertex_discover {};
struct on_vertex_examine {};
struct on_vertex_finish {};

struct is_vertex_discovered {};

struct on_edge_examine {};
struct on_edge_finish {};

struct on_tree_edge {};
struct on_non_tree_edge {};
struct on_forward_or_cross_edge {};
struct on_back_edge {};

struct on_edge_relaxed {};
struct on_edge_not_relaxed {};
struct on_edge_minimized {};
struct on_edge_not_minimized {};

}  // namespace ac