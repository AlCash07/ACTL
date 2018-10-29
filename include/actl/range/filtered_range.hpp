/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/range/range_facade.hpp>
#include <actl/traits/iterator_traits.hpp>
#include <actl/traits/type_traits.hpp>
#include <actl/util/compressed_pair.hpp>

namespace ac {

template <class Range, class Predicate>
class filter_iterator;

template <class Range, class Predicate>
class filtered_range
    : public range_facade<filtered_range<Range, Predicate>, filter_iterator<Range, Predicate>> {
public:
    using iterator = filter_iterator<Range, Predicate>;

    explicit filtered_range(Range range, Predicate pred) : data_(range, pred) {}

    iterator begin() const { return {original().begin(), *this}; }
    iterator end() const { return {original().end(), *this}; }

    const Range& original() const { return data_.first(); }

    bool evaluate(typename iterator::reference x) const { return data_.second()(x); }

private:
    compressed_pair<Range, Predicate> data_;
};

namespace detail {

template <class R, class P, class It = typename remove_cvref_t<R>::iterator>
using filter_iterator_base =
    iterator_adaptor<filter_iterator<R, P>, It,
                     std::conditional_t<is_random_access_iterator_v<It>,
                                        std::bidirectional_iterator_tag, use_default>>;

}  // namespace detail

template <class R, class P>
class filter_iterator : public detail::filter_iterator_base<R, P> {
    using base_t = detail::filter_iterator_base<R, P>;

public:
    filter_iterator(typename remove_cvref_t<R>::iterator it, const filtered_range<R, P>& range)
        : base_t(it), range_(range) {
        find_next();
    }

private:
    friend struct iterator_core_access;

    void find_next() {
        while (this->base() != range_.original().end() && !range_.evaluate(*this->base()))
            ++this->base_ref();
    }

    void increment() {
        ++this->base_ref();
        find_next();
    }

    void decrement() {
        do {
            --this->base_ref();
        } while (!range_.evaluate(*this->base()));
    }

    const filtered_range<R, P>& range_;
};

template <class Range, class Predicate>
inline auto filter_range(Range&& range, Predicate pred) {
    return filtered_range<remove_rvalue_ref_t<Range>, Predicate>(std::forward<Range>(range), pred);
}

}  // namespace ac
