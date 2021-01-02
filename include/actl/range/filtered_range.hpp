// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/iterator/facade/iterator_adaptor.hpp>
#include <actl/iterator/traits.hpp>
#include <actl/range/facade/range_facade.hpp>
#include <actl/traits/type_traits.hpp>
#include <actl/utility/compressed_pair.hpp>

namespace ac {

template <class Range, class Predicate>
class filtered_range;

namespace detail {

template <class R, class P>
struct filtered_range_types {
    using Iter = iterator_t<std::remove_reference_t<R>>;

    struct iter_types {
        using iterator_category =
            std::conditional_t<is_random_access_iterator_v<Iter>, std::bidirectional_iterator_tag,
                               iterator_category_t<Iter>>;
    };

    class iterator : public iterator_adaptor<iterator, Iter, iter_types> {
    public:
        explicit iterator(iterator_t<std::remove_reference_t<R>> iter,
                          const filtered_range<R, P>& range)
            : iterator_adaptor<iterator, Iter, iter_types>{iter}, range_{range} {
            find_next();
        }

    private:
        friend struct ac::iterator_core_access;

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

    using size_type = size_type_t<remove_cvref_t<R>>;
};

}  // namespace detail

template <class Range, class Predicate>
class filtered_range : public range_facade<filtered_range<Range, Predicate>,
                                           detail::filtered_range_types<Range, Predicate>> {
public:
    using iterator = typename detail::filtered_range_types<Range, Predicate>::iterator;

    explicit filtered_range(Range&& range, Predicate pred)
        : data_{std::forward<Range>(range), pred} {}

    auto begin() const {
        return iterator{original().begin(), *this};
    }
    auto end() const {
        return iterator{original().end(), *this};
    }

    const Range& original() const {
        return data_.first();
    }

    bool evaluate(reference_t<iterator> x) const {
        return data_.second()(x);
    }

private:
    compressed_pair<Range, Predicate> data_;
};

template <class Range, class Predicate>
auto filter_range(Range&& range, Predicate pred) {
    return filtered_range<Range, Predicate>{std::forward<Range>(range), pred};
}

}  // namespace ac
