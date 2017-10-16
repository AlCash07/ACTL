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
    using base_type = iterator_adaptor<limited_input_iterator<Iterator>, Iterator>;

public:
    limited_input_iterator() : base_type(Iterator{}), limit_{0} {}

    explicit limited_input_iterator(const Iterator& it, int limit) : base_type(it), limit_{limit} {}

private:
    friend struct iterator_core_access;

    void increment() {
        ACTL_ASSERT(limit_ > 0);
        if (--limit_ > 0) ++this->base();
        else this->base() = Iterator{};
    }

    int limit_;
};

template <class Iterator>
inline auto make_limited_input_iterator(Iterator it, int limit) {
    return limited_input_iterator<Iterator>(it, limit);
}

}  // namespace ac
