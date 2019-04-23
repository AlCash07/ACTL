/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/iterator/iterator_adaptor.hpp>

namespace ac {

/**
 * Input iterator wrapper that reads only a limited number of elements.
 */
template <class Iterator>
class limited_input_iterator : public iterator_adaptor<limited_input_iterator<Iterator>, Iterator> {
    using base_t = iterator_adaptor<limited_input_iterator<Iterator>, Iterator>;

public:
    explicit limited_input_iterator() : base_t{Iterator{}}, limit_{0} {}

    explicit limited_input_iterator(const Iterator& it, int limit) : base_t{it}, limit_{limit} {}

private:
    void increment() {
        ACTL_ASSERT(limit_ > 0);
        if (--limit_ > 0) {
            ++this->base_ref();
        } else {
            this->base_ref() = Iterator{};
        }
    }

    int limit_;

    friend struct iterator_core_access;
};

}  // namespace ac
