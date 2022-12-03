// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/iterator/facade/iterator_adaptor.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

/// Iterator that moves from the last element to the first and vice versa.
template <class Range>
class cyclic_iterator
    : public iterator_adaptor<cyclic_iterator<Range>, range_iterator_t<Range>>
{
    using Iter = range_iterator_t<Range>;

    Iter& iter()
    {
        return this->base_ref();
    }

public:
    explicit cyclic_iterator(Range& range, Iter iter)
        : iterator_adaptor<cyclic_iterator<Range>, Iter>{iter}, range_{&range}
    {
        AC_ASSERT(!std::empty(range));
        if (iter == end())
            this->base_ref() = begin();
    }

    cyclic_iterator& operator++()
    {
        ++iter();
        if (iter() == end())
            iter() = begin();
        return *this;
    }

    cyclic_iterator& operator--()
    {
        if (iter() == begin())
            iter() = end();
        --iter();
        return *this;
    }

    cyclic_iterator& operator+=(std::iter_difference_t<Iter> n)
    {
        auto cycle =
            static_cast<std::iter_difference_t<Iter>>(ranges::size(*range_));
        AC_ASSERT(abs(n) < cycle);
        if (n > 0)
        {
            iter() += n - (n >= (end() - iter()) ? cycle : 0);
        }
        else
        {
            iter() += n + (-n > (iter() - begin()) ? cycle : 0);
        }
        return *this;
    }

    friend std::iter_difference_t<Iter> operator-(
        cyclic_iterator const& lhs, cyclic_iterator const& rhs)
    {
        auto dist = lhs.base() - rhs.base();
        return dist >= 0 ? dist
                         : dist + static_cast<std::iter_difference_t<Iter>>(
                                      ranges::size(*lhs.range_));
    }

private:
    Iter begin() const
    {
        return ranges::begin(*range_);
    }

    Iter end() const
    {
        return ranges::end(*range_);
    }

    Range* range_;
};

template <class Range>
auto cyclic_begin(Range& range)
{
    return cyclic_iterator{range, ranges::begin(range)};
}

} // namespace ac
