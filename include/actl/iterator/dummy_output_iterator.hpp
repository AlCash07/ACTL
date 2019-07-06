/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <iterator>

namespace ac {

struct dummy_output_iterator {
    using R = dummy_output_iterator&;

    using iterator_category = std::output_iterator_tag;
    using value_type        = void;
    using reference         = void;
    using pointer           = void;
    using difference_type   = void;

    template <class T>
    R operator = (const T&) { return *this; }

    R operator*() { return *this; }
    R operator++() { return *this; }
    R operator++(int) { return *this; }
};

}  // namespace ac
