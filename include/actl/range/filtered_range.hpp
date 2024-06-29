// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/memory/no_unique_address.hpp>
#include <actl/range/interface/range_interface_selector.hpp>
#include <actl/range/iterator/interface/iterator_adaptor.hpp>

namespace ac {

namespace detail {

template <class Iter>
using filtered_iterator_category_t = std::conditional_t<
    std::random_access_iterator<Iter>,
    std::bidirectional_iterator_tag,
    iter_category_t<Iter>>;

} // namespace detail

template <class Range, class Predicate>
class filtered_range
    : public range_interface_selector_t<
          filtered_range<Range, Predicate>,
          detail::filtered_iterator_category_t<range_iterator_t<Range>>> {
    using Iter = range_iterator_t<Range>;
    using iterator_category =
        detail::filtered_iterator_category_t<range_iterator_t<Range>>;

public:
    class iterator
        : public iterator_adaptor<iterator, Iter, iterator_category> {
    public:
        // Required because std::sentinel_for requires std::semiregular.
        iterator() = default;

        explicit iterator(
            range_iterator_t<Range> iter, filtered_range const& range
        )
            : iterator_adaptor<iterator, Iter, iterator_category>{iter}
            , range_{&range} {
            find_next();
        }

        iterator& operator++() {
            ++this->base_ref();
            find_next();
            return *this;
        }

        iterator& operator--() {
            do {
                --this->base_ref();
            } while (!range_->evaluate(*this->base()));
            return *this;
        }

    private:
        void find_next() {
            while (this->base() != range_->original().end() &&
                   !range_->evaluate(*this->base()))
                ++this->base_ref();
        }

        // Pointer is used instead of a reference to support copy assignment
        // required for std::copyable.
        filtered_range const* range_;
    };

    explicit filtered_range(Range&& range, Predicate pred)
        : range_{std::forward<Range>(range)}, pred_{std::move(pred)} {}

    iterator begin() const {
        return iterator{original().begin(), *this};
    }

    iterator end() const {
        return iterator{original().end(), *this};
    }

    Range const& original() const {
        return range_;
    }

    bool evaluate(std::iter_reference_t<iterator> x) const {
        return pred_(x);
    }

private:
    AC_NO_UNIQUE_ADDRESS Range range_;
    AC_NO_UNIQUE_ADDRESS Predicate pred_;
};

template <class Range, class Predicate>
auto filter_range(Range&& range, Predicate pred) {
    return filtered_range<Range, Predicate>{
        std::forward<Range>(range), std::move(pred)
    };
}

} // namespace ac
