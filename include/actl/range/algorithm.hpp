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
Function for_each(Range& range, Function f) {
    return std::for_each(ranges::begin(range), ranges::end(range), f);
}

template<typename Range, typename T>
range_difference_t<Range> count(Range const& range, T const& value) {
    return std::count(ranges::begin(range), ranges::end(range), value);
}

template<typename RangeL, typename RangeR>
std::pair<range_iterator_t<RangeL>, range_iterator_t<RangeR>> mismatch(
    RangeL const& l, RangeR const& r
) {
    return std::mismatch(
        ranges::begin(l), ranges::end(l), ranges::begin(r), ranges::end(r)
    );
}

template<typename Range, typename T>
void fill(Range& target, T const& value) {
    std::fill(ranges::begin(target), ranges::end(target), value);
}

template<typename Range, typename Generator>
void generate(Range& target, Generator gen) {
    std::generate(ranges::begin(target), ranges::end(target), gen);
}

template<typename Range>
range_iterator_t<Range> unique(Range& range) {
    return std::unique(ranges::begin(range), ranges::end(range));
}

template<typename Range, typename BinaryPredicate>
range_iterator_t<Range> unique(Range& range, BinaryPredicate pred) {
    return std::unique(ranges::begin(range), ranges::end(range), pred);
}

template<typename Range>
void reverse(Range& range) {
    std::reverse(ranges::begin(range), ranges::end(range));
}

template<typename Range, typename UniformRandomNumberGenerator>
void shuffle(Range& range, UniformRandomNumberGenerator&& g) {
    std::shuffle(ranges::begin(range), ranges::end(range), g);
}

template<typename Range, typename Predicate>
range_iterator_t<Range> partition(Range& range, Predicate pred) {
    return std::partition(ranges::begin(range), ranges::end(range), pred);
}

template<typename Range>
void sort(Range&& range) {
    std::sort(ranges::begin(range), ranges::end(range));
}

template<typename Range, typename Compare>
void sort(Range&& range, Compare comp) {
    std::sort(ranges::begin(range), ranges::end(range), comp);
}

template<typename Range, typename T>
range_iterator_t<Range const> lower_bound(Range const& range, T const& value) {
    return std::lower_bound(ranges::begin(range), ranges::end(range), value);
}

template<typename Range, typename T, typename Compare>
range_iterator_t<Range const> lower_bound(
    Range const& range, T const& value, Compare comp
) {
    return std::lower_bound(
        ranges::begin(range), ranges::end(range), value, comp
    );
}

template<typename Range, typename T>
range_iterator_t<Range const> upper_bound(Range const& range, T const& value) {
    return std::upper_bound(ranges::begin(range), ranges::end(range), value);
}

template<typename Range, typename T, typename Compare>
range_iterator_t<Range const> upper_bound(
    Range const& range, T const& value, Compare comp
) {
    return std::upper_bound(
        ranges::begin(range), ranges::end(range), value, comp
    );
}

template<typename RangeL, typename RangeR, typename OutIter>
bool merge(RangeL const& l, RangeR const& r, OutIter output) {
    return std::merge(
        ranges::begin(l),
        ranges::end(l),
        ranges::begin(r),
        ranges::end(r),
        output
    );
}

template<typename RangeL, typename RangeR, typename OutIter, typename Compare>
bool merge(RangeL const& l, RangeR const& r, OutIter output, Compare comp) {
    return std::merge(
        ranges::begin(l),
        ranges::end(l),
        ranges::begin(r),
        ranges::end(r),
        output,
        comp
    );
}

template<typename Range>
range_iterator_t<Range const> min_element(Range const& range) {
    return std::min_element(ranges::begin(range), ranges::end(range));
}

template<typename Range, typename Compare>
range_iterator_t<Range const> min_element(Range const& range, Compare comp) {
    return std::min_element(ranges::begin(range), ranges::end(range), comp);
}

template<typename Range>
range_iterator_t<Range const> max_element(Range const& range) {
    return std::max_element(ranges::begin(range), ranges::end(range));
}

template<typename Range, typename Compare>
range_iterator_t<Range const> max_element(Range const& range, Compare comp) {
    return std::max_element(ranges::begin(range), ranges::end(range), comp);
}

template<typename Range, typename Iterator = range_iterator_t<Range const>>
std::pair<Iterator, Iterator> minmax_element(Range const& range) {
    return std::minmax_element(ranges::begin(range), ranges::end(range));
}

template<
    typename Range,
    typename Compare,
    typename Iterator = range_iterator_t<Range const>>
std::pair<Iterator, Iterator> minmax_element(Range const& range, Compare comp) {
    return std::minmax_element(ranges::begin(range), ranges::end(range), comp);
}

template<typename RangeL, typename RangeR>
bool lexicographical_compare(RangeL const& l, RangeR const& r) {
    return std::lexicographical_compare(
        ranges::begin(l), ranges::end(l), ranges::begin(r), ranges::end(r)
    );
}

template<typename RangeL, typename RangeR, typename Compare>
bool lexicographical_compare(RangeL const& l, RangeR const& r, Compare comp) {
    return std::lexicographical_compare(
        ranges::begin(l), ranges::end(l), ranges::begin(r), ranges::end(r), comp
    );
}

template<typename Range>
bool next_permutation(Range& range) {
    return std::next_permutation(ranges::begin(range), ranges::end(range));
}

template<typename Range, typename Compare>
bool next_permutation(Range& range, Compare comp) {
    return std::next_permutation(
        ranges::begin(range), ranges::end(range), comp
    );
}

} // namespace ac
