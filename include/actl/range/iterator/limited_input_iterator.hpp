// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/range/iterator/interface/iterator_adaptor.hpp>

namespace ac {

/// Input iterator wrapper that reads only a limited number of elements.
template<class Iterator>
class limited_input_iterator
    : public iterator_adaptor<limited_input_iterator<Iterator>, Iterator> {
    using base_t = iterator_adaptor<limited_input_iterator<Iterator>, Iterator>;

public:
    explicit limited_input_iterator() : base_t{Iterator{}}, m_limit{0} {}

    explicit limited_input_iterator(Iterator const& iter, int limit)
        : base_t{iter}, m_limit{limit} {}

    limited_input_iterator& operator++() {
        AC_ASSERT(m_limit > 0);
        if (--m_limit > 0)
            ++this->base_ref();
        else
            this->base_ref() = Iterator{};
        return *this;
    }

private:
    int m_limit;
};

} // namespace ac
