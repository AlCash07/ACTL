/***************************************************************************************************
 * Two iterators incapsulated to represent a range. Adopted from boost:
 * http://www.boost.org/doc/libs/1_65_1/libs/range/doc/html/range/reference/utilities/iterator_range.html
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/range/range_facade.hpp>

namespace ac {

template <class It>
class iterator_range : public range_facade<iterator_range<It>, It> {
public:
    iterator_range() = default;

    iterator_range(It begin, It end) : begin_{begin}, end_{end} {}

    It begin() const { return begin_; }
    It end() const { return end_; }

private:
    It begin_;
    It end_;
};

template <class Iterator>
inline auto make_range(Iterator first, Iterator last) {
    return iterator_range<Iterator>{first, last};
}

template <class Iterator, class Int>
inline auto make_range(Iterator first, Int n) {
    return iterator_range<Iterator>{first, std::next(first, n)};
}

}  // namespace ac
