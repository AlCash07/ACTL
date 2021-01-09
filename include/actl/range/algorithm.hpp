// Range overloads for the most common standard algorithms.
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/range.hpp>
#include <actl/traits/dependent.hpp>
#include <algorithm>

namespace ac {

template <class Range, class Function>
Function for_each(Range& dst, Function f) {
    return std::for_each(std::begin(dst), std::end(dst), f);
}

template <class Range, class T>
difference_t<Range> count(const Range& src, const T& value) {
    return std::count(std::begin(src), std::end(src), value);
}

template <class Range0, class Range1>
std::pair<iterator_t<Range0>, iterator_t<Range1>> mismatch(const Range0& lhs, const Range1& rhs) {
    return std::mismatch(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs));
}

template <class Range, class T>
void fill(Range& dst, const T& value) {
    std::fill(std::begin(dst), std::end(dst), value);
}

template <class Range, class Generator>
void generate(Range& dst, Generator gen) {
    std::generate(std::begin(dst), std::end(dst), gen);
}

template <class Range>
iterator_t<Range> unique(Range& dst) {
    return std::unique(std::begin(dst), std::end(dst));
}

template <class Range, class BinaryPredicate>
iterator_t<Range> unique(Range& dst, BinaryPredicate pred) {
    return std::unique(std::begin(dst), std::end(dst), pred);
}

template <class Range>
void reverse(Range& dst) {
    std::reverse(std::begin(dst), std::end(dst));
}

template <class Range, class UniformRandomNumberGenerator>
void shuffle(Range& dst, UniformRandomNumberGenerator&& g) {
    std::shuffle(std::begin(dst), std::end(dst), g);
}

template <class Range, class Predicate>
iterator_t<Range> partition(Range& dst, Predicate pred) {
    return std::partition(std::begin(dst), std::end(dst), pred);
}

template <class Range>
void sort(Range&& dst) {
    std::sort(std::begin(dst), std::end(dst));
}

template <class Range, class Compare>
void sort(Range&& dst, Compare comp) {
    std::sort(std::begin(dst), std::end(dst), comp);
}

template <class Range, class T>
iterator_t<const Range> lower_bound(const Range& src, const T& value) {
    return std::lower_bound(std::begin(src), std::end(src), value);
}

template <class Range, class T, class Compare>
iterator_t<const Range> lower_bound(const Range& src, const T& value, Compare comp) {
    return std::lower_bound(std::begin(src), std::end(src), value, comp);
}

template <class Range, class T>
iterator_t<const Range> upper_bound(const Range& src, const T& value) {
    return std::upper_bound(std::begin(src), std::end(src), value);
}

template <class Range, class T, class Compare>
iterator_t<const Range> upper_bound(const Range& src, const T& value, Compare comp) {
    return std::upper_bound(std::begin(src), std::end(src), value, comp);
}

template <class Range0, class Range1, class OutIter>
bool merge(const Range0& lhs, const Range1& rhs, OutIter dst) {
    return std::merge(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs), dst);
}

template <class Range0, class Range1, class OutIter, class Compare>
bool merge(const Range0& lhs, const Range1& rhs, OutIter dst, Compare comp) {
    return std::merge(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs), dst, comp);
}

template <class Range>
iterator_t<const Range> min_element(const Range& src) {
    return std::min_element(std::begin(src), std::end(src));
}

template <class Range, class Compare>
iterator_t<const Range> min_element(const Range& src, Compare comp) {
    return std::min_element(std::begin(src), std::end(src), comp);
}

template <class Range>
iterator_t<const Range> max_element(const Range& src) {
    return std::max_element(std::begin(src), std::end(src));
}

template <class Range, class Compare>
iterator_t<const Range> max_element(const Range& src, Compare comp) {
    return std::max_element(std::begin(src), std::end(src), comp);
}

template <class Range, class Iterator = iterator_t<const Range>>
std::pair<Iterator, Iterator> minmax_element(const Range& src) {
    return std::minmax_element(std::begin(src), std::end(src));
}

template <class Range, class Compare, class Iterator = iterator_t<const Range>>
std::pair<Iterator, Iterator> minmax_element(const Range& src, Compare comp) {
    return std::minmax_element(std::begin(src), std::end(src), comp);
}

template <class Range0, class Range1>
bool lexicographical_compare(const Range0& lhs, const Range1& rhs) {
    return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs),
                                        std::end(rhs));
}

template <class Range0, class Range1, class Compare>
bool lexicographical_compare(const Range0& lhs, const Range1& rhs, Compare comp) {
    return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs),
                                        std::end(rhs), comp);
}

template <class Range>
bool next_permutation(Range& dst) {
    return std::next_permutation(std::begin(dst), std::end(dst));
}

template <class Range, class Compare>
bool next_permutation(Range& dst, Compare comp) {
    return std::next_permutation(std::begin(dst), std::end(dst), comp);
}

}  // namespace ac
