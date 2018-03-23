/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/graph/detail/edge_list.hpp>

namespace ac {

template <class Directed,
          class EdgeContainer = std::vector<none>,
          class VertexContainer = std::vector<none>>
class edge_list {
public:
    using vertex_id = container_id_t<VertexContainer>;

private:
    generic_container<VertexContainer> vertices_;
};

}  // namespace ac
