/***************************************************************************************************
 * Range overloads for the most common standard algorithms.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <algorithm>

namespace ac {

template <class Range, class Function>
inline Function for_each(Range& dst, Function f) {
    return std::for_each(dst.begin(), dst.end(), f);
}

template <class Range, class T>
inline typename Range::difference_type count(const Range& src, const T& value) {
    return std::count(src.begin(), src.end(), value);
}

template <class Range, class OutputIterator>
inline OutputIterator copy(const Range& src, OutputIterator dst) {
    return std::copy(src.begin(), src.end(), dst);
}

template <class Range, class T>
inline void fill(Range& dst, const T& value) {
    std::fill(dst.begin(), dst.end(), value);
}

template <class Range, class Generator>
inline void generate(Range& dst, Generator gen) {
    std::generate(dst.begin(), dst.end(), gen);
}

template <class Range>
inline typename Range::iterator unique(Range& dst) {
    return std::unique(dst.begin(), dst.end());
}

template <class Range, class BinaryPredicate>
inline typename Range::iterator unique(Range& dst, BinaryPredicate pred) {
    return std::unique(dst.begin(), dst.end(), pred);
}

template <class Range>
inline void reverse(Range& dst) {
    std::reverse(dst.begin(), dst.end());
}

template <class Range, class UniformRandomNumberGenerator>
inline void shuffle(Range& dst, UniformRandomNumberGenerator&& g) {
    std::shuffle(dst.begin(), dst.end(), g);
}

template <class Range, class Predicate>
inline typename Range::iterator partition(Range& dst, Predicate pred) {
    return std::partition(dst.begin(), dst.end(), pred);
}

template <class Range>
inline void sort(Range& dst) {
    std::sort(dst.begin(), dst.end());
}

template <class Range, class Compare>
inline void sort(Range& dst, Compare comp) {
    std::sort(dst.begin(), dst.end(), comp);
}

template <class Range, class T>
inline typename Range::iterator lower_bound(const Range& src, const T& value) {
    return std::lower_bound(src.begin(), src.end(), value);
}

template <class Range, class T, class Compare>
inline typename Range::iterator lower_bound(const Range& src, const T& value, Compare comp) {
    return std::lower_bound(src.begin(), src.end(), value, comp);
}

template <class Range, class T>
inline typename Range::iterator upper_bound(const Range& src, const T& value) {
    return std::upper_bound(src.begin(), src.end(), value);
}

template <class Range, class T, class Compare>
inline typename Range::iterator upper_bound(const Range& src, const T& value, Compare comp) {
    return std::upper_bound(src.begin(), src.end(), value, comp);
}

template <class Range>
inline typename Range::iterator min_element(const Range& src) {
    return std::min_element(src.begin(), src.end());
}

template <class Range, class Compare>
inline typename Range::iterator min_element(const Range& src, Compare comp) {
    return std::min_element(src.begin(), src.end(), comp);
}

template <class Range>
inline typename Range::iterator max_element(const Range& src) {
    return std::max_element(src.begin(), src.end());
}

template <class Range, class Compare>
inline typename Range::iterator max_element(const Range& src, Compare comp) {
    return std::max_element(src.begin(), src.end(), comp);
}

template <class Range, class Iterator = typename Range::iterator>
inline std::pair<Iterator, Iterator> minmax_element(const Range& src) {
    return std::minmax_element(src.begin(), src.end());
}

template <class Range, class Compare, class Iterator = typename Range::iterator>
inline std::pair<Iterator, Iterator> minmax_element(const Range& src, Compare comp) {
    return std::minmax_element(src.begin(), src.end(), comp);
}

template <class Range0, class Range1>
inline bool lexicographical_compare(const Range0& lhs, const Range1& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class Range0, class Range1, class Compare>
inline bool lexicographical_compare(const Range0& lhs, const Range1& rhs, Compare comp) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), comp);
}

template <class Range>
inline bool next_permutation(Range& dst) {
    return std::next_permutation(dst.begin(), dst.end());
}

template <class Range, class Compare>
inline bool next_permutation(Range& dst, Compare comp) {
    return std::next_permutation(dst.begin(), dst.end(), comp);
}

}  // namespace ac
