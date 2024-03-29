// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/memory/no_unique_address.hpp>
#include <actl/range/interface/range_interface_selector.hpp>
#include <actl/range/iterator/interface/iterator_adaptor.hpp>
#include <actl/range/iterator/traits/is_const_iterator.hpp>

namespace ac {

template <class Range, class Predicate>
class filtered_range;

namespace detail {

template <class R, class P>
struct filtered_range_types
{
    using Iter = range_iterator_t<R>;

    using iterator_category = std::conditional_t<
        std::random_access_iterator<Iter>,
        std::bidirectional_iterator_tag,
        iter_category_t<Iter>>;

    class iterator : public iterator_adaptor<iterator, Iter, iterator_category>
    {
    public:
        // Required because std::sentinel_for requires std::semiregular.
        iterator() = default;

        explicit iterator(
            range_iterator_t<R> iter, filtered_range<R, P> const& range)
            : iterator_adaptor<iterator, Iter, iterator_category>{iter}
            , range_{&range}
        {
            find_next();
        }

        iterator& operator++()
        {
            ++this->base_ref();
            find_next();
            return *this;
        }

        iterator& operator--()
        {
            do
            {
                --this->base_ref();
            }
            while (!range_->evaluate(*this->base()));
            return *this;
        }

    private:
        void find_next()
        {
            while (this->base() != range_->original().end() &&
                   !range_->evaluate(*this->base()))
                ++this->base_ref();
        }

        // Pointer is used instead of a reference to support copy assignment
        // required for std::copyable.
        filtered_range<R, P> const* range_;
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
        : range_{std::forward<Range>(range)}, pred_{std::move(pred)}
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
        return range_;
    }

    bool evaluate(std::iter_reference_t<iterator> x) const
    {
        return pred_(x);
    }

private:
    AC_NO_UNIQUE_ADDRESS Range range_;
    AC_NO_UNIQUE_ADDRESS Predicate pred_;
};

template <class Range, class Predicate>
auto filter_range(Range&& range, Predicate pred)
{
    return filtered_range<Range, Predicate>{
        std::forward<Range>(range), std::move(pred)};
}

} // namespace ac
