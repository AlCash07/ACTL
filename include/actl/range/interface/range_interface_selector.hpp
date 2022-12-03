// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/interface/random_access_range_interface.hpp>

namespace ac {

template <class Range, class Types, class Category>
struct range_interface_selector
{
    using type = basic_range_interface<Range, Types>;
};

template <class Range, class Types>
struct range_interface_selector<Range, Types, std::bidirectional_iterator_tag>
{
    using type = bidirectional_range_interface<Range, Types>;
};

template <class Range, class Types>
struct range_interface_selector<Range, Types, std::random_access_iterator_tag>
{
    using type = random_access_range_interface<Range, Types>;
};

template <
    class Range,
    class Types,
    class Category = iter_category_t<typename Types::iterator>>
using range_interface_selector_t =
    typename range_interface_selector<Range, Types, Category>::type;

} // namespace ac
