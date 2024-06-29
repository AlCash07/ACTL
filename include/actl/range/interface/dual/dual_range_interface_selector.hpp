// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/interface/dual/random_access_dual_range_interface.hpp>

namespace ac {

template<class Range, class Category>
struct dual_range_interface_selector {
    using type = basic_dual_range_interface<Range>;
};

template<class Range>
struct dual_range_interface_selector<Range, std::bidirectional_iterator_tag> {
    using type = bidirectional_range_interface<Range>;
};

template<class Range>
struct dual_range_interface_selector<Range, std::random_access_iterator_tag> {
    using type = random_access_range_interface<Range>;
};

template<
    class Range,
    class Category = iter_category_t<typename Types::iterator>>
using dual_range_interface_selector_t =
    typename dual_range_interface_selector<Range, Category>::type;

} // namespace ac
