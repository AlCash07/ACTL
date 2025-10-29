// Range overloads for the most common standard algorithms.
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/traits/associated_types.hpp>
#include <actl/range/traits/concepts.hpp>
#include <algorithm>

namespace ac {

template<typename Range, typename Function>
Function for_each(Range& dst, Function f) {
    return std::for_each(ranges::begin(dst), ranges::end(dst), f);
}

template<typename Range, typename T>
range_difference_t<Range> count(Range const& src, T const& value) {
    return std::count(ranges::begin(src), ranges::end(src), value);
}

template<typename Range0, typename Range1>
std::pair<range_iterator_t<Range0>, range_iterator_t<Range1>> mismatch(
    Range0 const& lhs, Range1 const& rhs
) {
    return std::mismatch(
        ranges::begin(lhs),
        ranges::end(lhs),
        ranges::begin(rhs),
        ranges::end(rhs)
    );
}

template<typename Range, typename T>
void fill(Range& dst, T const& value) {
    std::fill(ranges::begin(dst), ranges::end(dst), value);
}

template<typename Range, typename Generator>
void generate(Range& dst, Generator gen) {
    std::generate(ranges::begin(dst), ranges::end(dst), gen);
}

template<typename Range>
range_iterator_t<Range> unique(Range& dst) {
    return std::unique(ranges::begin(dst), ranges::end(dst));
}

template<typename Range, typename BinaryPredicate>
range_iterator_t<Range> unique(Range& dst, BinaryPredicate pred) {
    return std::unique(ranges::begin(dst), ranges::end(dst), pred);
}

template<typename Range>
void reverse(Range& dst) {
    std::reverse(ranges::begin(dst), ranges::end(dst));
}

template<typename Range, typename UniformRandomNumberGenerator>
void shuffle(Range& dst, UniformRandomNumberGenerator&& g) {
    std::shuffle(ranges::begin(dst), ranges::end(dst), g);
}

template<typename Range, typename Predicate>
range_iterator_t<Range> partition(Range& dst, Predicate pred) {
    return std::partition(ranges::begin(dst), ranges::end(dst), pred);
}

template<typename Range>
void sort(Range&& dst) {
    std::sort(ranges::begin(dst), ranges::end(dst));
}

template<typename Range, typename Compare>
void sort(Range&& dst, Compare comp) {
    std::sort(ranges::begin(dst), ranges::end(dst), comp);
}

template<typename Range, typename T>
range_iterator_t<Range const> lower_bound(Range const& src, T const& value) {
    return std::lower_bound(ranges::begin(src), ranges::end(src), value);
}

template<typename Range, typename T, typename Compare>
range_iterator_t<Range const> lower_bound(
    Range const& src, T const& value, Compare comp
) {
    return std::lower_bound(ranges::begin(src), ranges::end(src), value, comp);
}

template<typename Range, typename T>
range_iterator_t<Range const> upper_bound(Range const& src, T const& value) {
    return std::upper_bound(ranges::begin(src), ranges::end(src), value);
}

template<typename Range, typename T, typename Compare>
range_iterator_t<Range const> upper_bound(
    Range const& src, T const& value, Compare comp
) {
    return std::upper_bound(ranges::begin(src), ranges::end(src), value, comp);
}

template<typename Range0, typename Range1, typename OutIter>
bool merge(Range0 const& lhs, Range1 const& rhs, OutIter dst) {
    return std::merge(
        ranges::begin(lhs),
        ranges::end(lhs),
        ranges::begin(rhs),
        ranges::end(rhs),
        dst
    );
}

template<typename Range0, typename Range1, typename OutIter, typename Compare>
bool merge(Range0 const& lhs, Range1 const& rhs, OutIter dst, Compare comp) {
    return std::merge(
        ranges::begin(lhs),
        ranges::end(lhs),
        ranges::begin(rhs),
        ranges::end(rhs),
        dst,
        comp
    );
}

template<typename Range>
range_iterator_t<Range const> min_element(Range const& src) {
    return std::min_element(ranges::begin(src), ranges::end(src));
}

template<typename Range, typename Compare>
range_iterator_t<Range const> min_element(Range const& src, Compare comp) {
    return std::min_element(ranges::begin(src), ranges::end(src), comp);
}

template<typename Range>
range_iterator_t<Range const> max_element(Range const& src) {
    return std::max_element(ranges::begin(src), ranges::end(src));
}

template<typename Range, typename Compare>
range_iterator_t<Range const> max_element(Range const& src, Compare comp) {
    return std::max_element(ranges::begin(src), ranges::end(src), comp);
}

template<typename Range, typename Iterator = range_iterator_t<Range const>>
std::pair<Iterator, Iterator> minmax_element(Range const& src) {
    return std::minmax_element(ranges::begin(src), ranges::end(src));
}

template<
    typename Range,
    typename Compare,
    typename Iterator = range_iterator_t<Range const>>
std::pair<Iterator, Iterator> minmax_element(Range const& src, Compare comp) {
    return std::minmax_element(ranges::begin(src), ranges::end(src), comp);
}

template<typename Range0, typename Range1>
bool lexicographical_compare(Range0 const& lhs, Range1 const& rhs) {
    return std::lexicographical_compare(
        ranges::begin(lhs),
        ranges::end(lhs),
        ranges::begin(rhs),
        ranges::end(rhs)
    );
}

template<typename Range0, typename Range1, typename Compare>
bool lexicographical_compare(
    Range0 const& lhs, Range1 const& rhs, Compare comp
) {
    return std::lexicographical_compare(
        ranges::begin(lhs),
        ranges::end(lhs),
        ranges::begin(rhs),
        ranges::end(rhs),
        comp
    );
}

template<typename Range>
bool next_permutation(Range& dst) {
    return std::next_permutation(ranges::begin(dst), ranges::end(dst));
}

template<typename Range, typename Compare>
bool next_permutation(Range& dst, Compare comp) {
    return std::next_permutation(ranges::begin(dst), ranges::end(dst), comp);
}

} // namespace ac
