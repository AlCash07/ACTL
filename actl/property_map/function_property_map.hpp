/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>
#include <type_traits>

namespace ac {

/**
 * Property map that applies given function to the key.
 */
template <class Function, class Key, class Return = std::result_of_t<const Function(const Key&)>>
class function_property_map : public computed_property_map<false, Key, Return> {
public:
    explicit function_property_map(Function f = Function{}) : f_{f} {}

    Return operator[](const Key& key) const { return f_(key); }

private:
    Function f_;
};

template <class Key, class Function>
inline auto make_function_property_map(const Function& f) {
    return function_property_map<Function, Key>(f);
}

}  // namespace ac
