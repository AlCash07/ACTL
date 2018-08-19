/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>
#include <actl/type/none.hpp>

namespace ac::detail {

template <class Key>
class none_property_map : public property_map<Key, none, none, false> {
    friend none get(const none_property_map& pm, Key key) { return none(); }
};

}  // namespace ac::detail
