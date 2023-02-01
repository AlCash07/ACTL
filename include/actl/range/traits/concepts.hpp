// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/access/all.hpp>
#include <actl/range/iterator/traits/category.hpp>

namespace ac {

// https://en.cppreference.com/w/cpp/ranges/range
template <class T>
concept Range = requires(T& t) {
                    ranges::begin(t);
                    ranges::end(t);
                };

// https://en.cppreference.com/w/cpp/ranges/input_range
template <class T>
concept InputRange = Range<T> && std::input_iterator<range_iterator_t<T>>;

// https://en.cppreference.com/w/cpp/ranges/forward_range
template <class T>
concept ForwardRange =
    InputRange<T> && std::forward_iterator<range_iterator_t<T>>;

// https://en.cppreference.com/w/cpp/ranges/bidirectional_range
template <class T>
concept BidirectionalRange =
    ForwardRange<T> && std::bidirectional_iterator<range_iterator_t<T>>;

// https://en.cppreference.com/w/cpp/ranges/random_access_range
template <class T>
concept RandomAccessRange =
    BidirectionalRange<T> && std::random_access_iterator<range_iterator_t<T>>;

// Not all ranges with data() are contiguous.
// For example, multi-dimensional arrays of rank > 1 are not.
// Instead of using std::add_pointer like in the standard
// https://en.cppreference.com/w/cpp/ranges/contiguous_range
// we just compare references.
// This is done to support custom pointers instead of just raw pointers.
template <class T>
concept ContiguousRange =
    RandomAccessRange<T> && std::contiguous_iterator<range_iterator_t<T>> &&
    requires(T& t) {
        {
            *ranges::data(t)
            } -> std::same_as<std::iter_reference_t<range_iterator_t<T>>>;
    };

} // namespace ac
