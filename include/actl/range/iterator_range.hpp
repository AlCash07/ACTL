// Two iterators incapsulated to represent a range. Adopted from boost:
// http://www.boost.org/doc/libs/1_65_1/libs/range/doc/html/range/reference/utilities/iterator_range.html
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/range/facade/range_facade.hpp>
#include <actl/range/traits.hpp>

namespace ac {

namespace detail {

template <class Iter>
struct iter_range_types {
    using iterator = Iter;
    using size_type = difference_type_t<Iter>;
};

}  // namespace detail

template <class Iter, class Traits = default_range_traits>
class iterator_range
    : public range_facade<iterator_range<Iter, Traits>, detail::iter_range_types<Iter>> {
public:
    constexpr iterator_range() = default;

    constexpr iterator_range(Iter begin, Iter end) : begin_{begin}, end_{end} {}

    constexpr Iter begin() const {
        return begin_;
    }
    constexpr Iter end() const {
        return end_;
    }

private:
    Iter begin_;
    Iter end_;
};

template <class Iter, class Traits>
struct range_traits<iterator_range<Iter, Traits>> : Traits {
    static constexpr bool is_container = false;
};

template <class Traits = default_range_traits, class Iterator>
auto make_range(Iterator first, Iterator last) {
    return iterator_range<Iterator, Traits>{first, last};
}

template <class Traits = default_range_traits, class Iterator, class Int>
auto make_range(Iterator first, Int n) {
    return iterator_range<Iterator, Traits>{first, std::next(first, n)};
}

template <class Container>
auto make_range(Container&& cont) {
    return make_range<range_traits<std::remove_reference_t<Container>>>(std::begin(cont),
                                                                        std::end(cont));
}

template <class Container>
auto make_crange(const Container& cont) {
    return make_range(cont);
}

}  // namespace ac
