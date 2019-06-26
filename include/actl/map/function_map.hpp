/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/map.hpp>
#include <actl/traits/function_traits.hpp>

namespace ac {

/**
 * Property map that applies given function to the key.
 */
template <class Function>
class function_map {
public:
    using key_type = argument_type_t<Function, 0>;
    using reference = return_type_t<Function>;

    explicit function_map(Function f = {}) : f_{f} {}

    friend reference get(const function_map& map, key_type key) { return map.f_(key); }

private:
    Function f_;
};

}  // namespace ac
