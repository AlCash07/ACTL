/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/property_map.hpp>
#include <actl/traits/function_traits.hpp>
#include <actl/traits/type_traits.hpp>

namespace ac {

/**
 * Property map that applies given function to the key.
 */
template <class Function, class Key = argument_type_t<Function, 0>,
          class Ref = return_type_t<Function>>
class function_map : public property_map<Key, remove_cvref_t<Ref>, Ref, false> {
public:
    explicit function_map(Function f = {}) : f_{f} {}

    friend Ref get(const function_map& map, Key key) { return map.f_(key); }

private:
    Function f_;
};

}  // namespace ac
