// Two iterators incapsulated to represent a range. Adopted from boost:
// http://www.boost.org/doc/libs/1_65_1/libs/range/doc/html/range/reference/utilities/iterator_range.html
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/interface/range_interface_selector.hpp>
#include <actl/range/traits/properties.hpp>
#include <actl/range/traits/super_range.hpp>

namespace ac {

template<class Iter, class Super = none>
class iterator_range
    : public range_interface_selector_t<
          iterator_range<Iter, Super>,
          iter_category_t<Iter>> {
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

template<class Iter, class Super>
struct super_range<iterator_range<Iter, Super>> {
    using type = Super;
};

template<class Iter, class Super>
struct range_properties<iterator_range<Iter, Super>> : range_properties<Super> {
    static constexpr bool is_container = false;
};

template<class SuperRange = none, class Iterator>
auto make_range(Iterator first, Iterator last) {
    return iterator_range<Iterator, SuperRange>{first, last};
}

template<class SuperRange = none, class Iterator, class Int>
auto make_range(Iterator first, Int n) {
    return iterator_range<Iterator, SuperRange>{first, std::next(first, n)};
}

template<class Container>
auto make_range(Container&& cont) {
    return make_range<std::remove_reference_t<Container>>(
        ranges::begin(cont), ranges::end(cont)
    );
}

template<class Container>
auto make_crange(Container const& cont) {
    return make_range(cont);
}

} // namespace ac
