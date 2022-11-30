// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/iterator/facade/iterator_adaptor.hpp>
#include <actl/iterator/traits/is_const_iterator.hpp>
#include <actl/meta/type_traits.hpp>
#include <actl/range/interface/range_interface.hpp>
#include <actl/utility/compressed_pair.hpp>

namespace ac {

template <class Range, class Predicate>
class filtered_range;

namespace detail {

template <class R, class P>
struct filtered_range_types
{
    using Iter = range_iterator_t<R>;

    struct iter_types : default_iterator_adaptor_types
    {
        using iterator_category = std::conditional_t<
            std::random_access_iterator<Iter>,
            std::bidirectional_iterator_tag,
            iter_category_t<Iter>>;
    };

    class iterator : public iterator_adaptor<iterator, Iter, iter_types>
    {
    public:
        explicit iterator(
            range_iterator_t<R> iter, filtered_range<R, P> const& range)
            : iterator_adaptor<iterator, Iter, iter_types>{iter}, range_{range}
        {
            find_next();
        }

    private:
        friend struct ac::iterator_core_access;

        void find_next()
        {
            while (this->base() != range_.original().end() &&
                   !range_.evaluate(*this->base()))
                ++this->base_ref();
        }

        void increment()
        {
            ++this->base_ref();
            find_next();
        }

        void decrement()
        {
            do
            {
                --this->base_ref();
            }
            while (!range_.evaluate(*this->base()));
        }

        filtered_range<R, P> const& range_;
    };

    using size_type = range_size_t<R>;
};

} // namespace detail

template <class Range, class Predicate>
class filtered_range
    : public range_interface_selector_t<
          filtered_range<Range, Predicate>,
          detail::filtered_range_types<Range, Predicate>>
{
public:
    using iterator =
        typename detail::filtered_range_types<Range, Predicate>::iterator;

    explicit filtered_range(Range&& range, Predicate pred)
        : data_{std::forward<Range>(range), std::move(pred)}
    {}

    auto begin() const
    {
        return iterator{original().begin(), *this};
    }

    auto end() const
    {
        return iterator{original().end(), *this};
    }

    Range const& original() const
    {
        return data_.first();
    }

    bool evaluate(std::iter_reference_t<iterator> x) const
    {
        return data_.second()(x);
    }

private:
    compressed_pair<Range, Predicate> data_;
};

template <class Range, class Predicate>
auto filter_range(Range&& range, Predicate pred)
{
    return filtered_range<Range, Predicate>{
        std::forward<Range>(range), std::move(pred)};
}

} // namespace ac
