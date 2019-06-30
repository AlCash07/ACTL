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
 * Map that applies given function to the key.
 */
template <class Function>
class function_map {
public:
    explicit function_map(Function f = {}) : f_{f} {}

    map_reference_t<function_map> get(map_key_t<function_map> key) const { return f_(key); }

private:
    Function f_;
};

template <class F>
struct const_map_traits<function_map<F>>
    : map_traits_base<argument_type_t<F, 0>, return_type_t<F>> {};

}  // namespace ac
