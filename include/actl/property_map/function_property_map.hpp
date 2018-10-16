/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>

namespace ac {

/**
 * Property map that applies given function to the key.
 */
template <class Function, class Key, class Ref = std::result_of_t<const Function(Key)>,
          class Value = remove_cvref_t<Ref>>
class function_property_map : public property_map<Key, Value, Ref, false> {
public:
    explicit function_property_map(Function f = Function{}) : f_{f} {}

    friend Ref get(const function_property_map& pm, Key key) { return pm.f_(key); }

private:
    Function f_;
};

// TODO: deduce key type from function signature.
template <class Key, class Function>
inline auto make_function_property_map(Function f) {
    return function_property_map<Function, Key>(f);
}

}  // namespace ac
