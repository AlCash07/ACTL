// https://en.cppreference.com/w/cpp/ranges/iterator_t
//
// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/associated_types.hpp>
#include <actl/range/access/all.hpp>
#include <actl/range/traits/category.hpp>
#include <iterator>

namespace ac {

template <class Range>
using range_iterator_t = decltype(ranges::begin(std::declval<Range&>()));

template <class Range>
using range_sentinel_t = decltype(ranges::end(std::declval<Range&>()));

template <class Range>
using range_size_t =
    unwrap_constant_t<decltype(ranges::size(std::declval<Range&>()))>;

template <class Range>
using range_difference_t = std::iter_difference_t<range_iterator_t<Range>>;

template <class Range>
using range_value_t = std::iter_value_t<range_iterator_t<Range>>;

template <class Range>
using range_reference_t = std::iter_reference_t<range_iterator_t<Range>>;

template <class Range>
requires is_range_v<Range>
struct associated_types<Range>
{
    using value_type = range_value_t<Range>;
    using reference = range_reference_t<Range>;
};

} // namespace ac
