// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/access/begin.hpp>
#include <actl/range/interface/random_access_range_interface.hpp>
#include <actl/range/iterator/traits/category.hpp>

namespace ac {

template<typename Range, typename Category>
struct range_interface_selector {
    using type = basic_range_interface<Range>;
};

template<typename Range>
struct range_interface_selector<Range, std::bidirectional_iterator_tag> {
    using type = bidirectional_range_interface<Range>;
};

template<typename Range>
struct range_interface_selector<Range, std::random_access_iterator_tag> {
    using type = random_access_range_interface<Range>;
};

template<
    typename Range,
    typename Category = iter_category_t<range_iterator_t<Range>>>
using range_interface_selector_t =
    typename range_interface_selector<Range, Category>::type;

} // namespace ac
