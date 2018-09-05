/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/limited_input_iterator.hpp>
#include <actl/range/range.hpp>

namespace ac {

template <class Iterator>
inline auto make_limited_input_range(Iterator it, int limit) {
    return make_range(make_limited_input_iterator(it, limit), limited_input_iterator<Iterator>());
}

}  // namespace ac