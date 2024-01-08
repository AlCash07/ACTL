// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/numeric/interval/interval_kind.hpp>

template <ac::endpoint_kind Expected, ac::interval_kind... Intervals>
constexpr void test_interval_begin() noexcept
{
    static_assert((... && (Expected == interval_begin(Intervals))));
}

template <ac::endpoint_kind Expected, ac::interval_kind... Intervals>
constexpr void test_interval_end() noexcept
{
    static_assert((... && (Expected == interval_end(Intervals))));
}

constexpr bool test_interval_endpoint_kinds() noexcept
{
    using namespace ac;
    /* interval_begin */
    test_interval_begin<
        endpoint_kind::open,
        interval_kind::open,
        interval_kind::open_closed,
        interval_kind::open_unbounded>();
    test_interval_begin<
        endpoint_kind::closed,
        interval_kind::closed_open,
        interval_kind::closed,
        interval_kind::closed_unbounded>();
    test_interval_begin<
        endpoint_kind::unbounded,
        interval_kind::unbounded_open,
        interval_kind::unbounded_closed,
        interval_kind::unbounded>();
    /* interval_end */
    test_interval_end<
        endpoint_kind::open,
        interval_kind::open,
        interval_kind::closed_open,
        interval_kind::unbounded_open>();
    test_interval_end<
        endpoint_kind::closed,
        interval_kind::open_closed,
        interval_kind::closed,
        interval_kind::unbounded_closed>();
    test_interval_end<
        endpoint_kind::unbounded,
        interval_kind::open_unbounded,
        interval_kind::closed_unbounded,
        interval_kind::unbounded>();
    return true;
}

static_assert(test_interval_endpoint_kinds());
