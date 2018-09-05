/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/transition_iterator.hpp>
#include <actl/range/range.hpp>

namespace ac {

template <class Iterator>
inline auto make_transition_range(Iterator first, Iterator last) {
    return make_range(make_transition_iterator(first, first, last),
                      make_transition_iterator(last, first, last));
}

template <class Iterator, class Int>
inline auto make_transition_range(Iterator first, Int n) {
    return make_transition_range(first, std::next(first, n));
}

}  // namespace ac
