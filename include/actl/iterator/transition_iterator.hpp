/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/iterator_facade.hpp>
#include <utility>

namespace ac {

/**
 * Iterator that stops only at non-default values (that convert to true) in the continuous memory,
 * treating it as a map from integer domain.
 */
// TODO: pass a predicate that indicates empty values.
template <class Iterator, class Diff = int,
          class Value = std::pair<Diff, typename std::iterator_traits<Iterator>::reference>>
class transition_iterator
    : public iterator_facade<transition_iterator<Iterator, Diff, Value>, std::input_iterator_tag,
                             Value, Value, const Value*, void> {
public:
    explicit transition_iterator() = default;

    transition_iterator(Iterator it, Iterator begin, Iterator end)
        : it_{it}, begin_{begin}, end_{end} {
        skip_empty();
    }

private:
    Value dereference() const { return {static_cast<Diff>(it_ - begin_), *it_}; }

    void skip_empty() { for (; it_ != end_ && !*it_; ++it_); }

    void increment() {
        ++it_;
        skip_empty();
    }

    bool equals(const transition_iterator& other) const { return it_ == other.it_; }

    Iterator it_;
    Iterator begin_;
    Iterator end_;

    friend struct iterator_core_access;
};

template <class Iterator>
inline auto make_transition_iterator(Iterator it, Iterator begin, Iterator end) {
    return transition_iterator<Iterator>(it, begin, end);
}

}  // namespace ac