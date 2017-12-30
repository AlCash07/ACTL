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

#include <actl/assert.hpp>
#include <iterator>

namespace ac {

template <class Iterator>
class range;

namespace detail {

template <class Iterator, class Category>
class range_base;

template <class It>
class range_base<It, std::forward_iterator_tag> : public std::iterator_traits<It> {
public:
    using iterator  = It;
    using size_type = std::size_t;
    // all other type aliases come from std::iterator_traits<It>

    It begin() const { return begin_; }

    It end() const { return end_; }

    bool empty() const { return begin_ == end_; }

    bool operator()() const { return !empty(); }

    typename std::iterator_traits<It>::reference front() const {
        ACTL_ASSERT(!empty());
        return *begin_;
    }

    void drop_front() {
        ACTL_ASSERT(!empty());
        ++begin_;
    }

protected:
    range_base() : begin_{}, end_{} {}

    template <class It1>
    range_base(It1 begin, It1 end) : begin_{begin}, end_{end} {}

    It begin_;
    It end_;
};

template <class It>
class range_base<It, std::input_iterator_tag> : public range_base<It, std::forward_iterator_tag> {
protected:
    range_base() = default;

    template <class It1>
    range_base(It1 first, It1 last) : range_base<It, std::forward_iterator_tag>(first, last) {}
};

template <class It>
class range_base<It, std::output_iterator_tag> : public range_base<It, std::forward_iterator_tag> {
protected:
    range_base() = default;

    template <class It1>
    range_base(It1 first, It1 last) : range_base<It, std::forward_iterator_tag>(first, last) {}
};

template <class It>
class range_base<It, std::bidirectional_iterator_tag>
    : public range_base<It, std::forward_iterator_tag> {
    using base_t = range_base<It, std::forward_iterator_tag>;

public:
    typename base_t::reference back() const {
        ACTL_ASSERT(!this->empty());
        auto end = this->end_;
        return *--end;
    }

    void drop_back() {
        ACTL_ASSERT(!this->empty());
        --this->end_;
    }

protected:
    range_base() = default;

    template <class It1>
    range_base(It1 first, It1 last) : base_t(first, last) {}
};

template <class It>
class range_base<It, std::random_access_iterator_tag>
    : public range_base<It, std::bidirectional_iterator_tag> {
    using base_t = range_base<It, std::bidirectional_iterator_tag>;

public:
    using typename base_t::size_type;

    typename base_t::reference operator[](typename base_t::difference_type at) const {
        ACTL_ASSERT(at >= 0);
        ACTL_ASSERT(static_cast<size_type>(at) < size());
        return this->begin_[at];
    }

    size_type size() const { return static_cast<size_type>(this->end_ - this->begin_); }

protected:
    range_base() = default;

    template <class It1>
    range_base(It1 first, It1 last) : base_t(first, last) {}
};

}  // namespace detail

template <class It>
class range : public detail::range_base<It, typename std::iterator_traits<It>::iterator_category> {
    using base_t = detail::range_base<It, typename std::iterator_traits<It>::iterator_category>;

public:
    range() = default;

    template <class Iterator>
    range(Iterator first, Iterator last) : base_t(first, last) {}
};

template <class Iterator>
inline auto make_range(Iterator first, Iterator last) {
    return range<Iterator>(first, last);
}

template <class Iterator, class Int>
inline auto make_range(Iterator first, Int n) {
    return range<Iterator>(first, std::next(first, n));
}

template <class Container>
inline auto make_range(Container& container) {
    return make_range(container.begin(), container.end());
}

template <class Container>
inline auto make_crange(Container& container) {
    return make_range(container.cbegin(), container.cend());
}

}  // namespace ac
