// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/iterator/limited_input_iterator.hpp>
#include <actl/range/iterator_range.hpp>

namespace ac {

template <class Iterator>
auto make_limited_input_range(Iterator iter, int limit)
{
    return make_range(
        limited_input_iterator{iter, limit},
        limited_input_iterator<Iterator>{});
}

} // namespace ac
