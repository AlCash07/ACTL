/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <utility>
#include <actl/iterator/iterator_facade.hpp>

namespace ac {

/**
 * Iterator that stops only at non-default values (that convert to true) in the continuous memory,
 * treating it as a map from integer domain.
 */
template <class Iterator,
          class Value = std::pair<const int, typename std::iterator_traits<Iterator>::reference>>
class transition_iterator
    : public iterator_facade<transition_iterator<Iterator, Value>,
                             std::input_iterator_tag,
                             Value,
                             Value,
                             const Value*,
                             typename std::iterator_traits<Iterator>::difference_type> {
public:
    transition_iterator() = default;

    transition_iterator(Iterator it, Iterator begin, Iterator end)
        : it_{it}, begin_{begin}, end_{end} {
        skip_empty();
    }

private:
    friend struct iterator_core_access;

    Value dereference() const { return {static_cast<const int>(it_ - begin_), *it_}; }

    void skip_empty() { for (; it_ != end_ && !*it_; ++it_); }

    void increment() {
        ++it_;
        skip_empty();
    }

    bool equals(const transition_iterator& other) const { return it_ == other.it_; }

    Iterator it_;
    Iterator begin_;
    Iterator end_;
};

template <class Iterator>
inline auto make_transition_iterator(Iterator it, Iterator begin, Iterator end) {
    return transition_iterator<Iterator>(it, begin, end);
}

}  // namespace ac
