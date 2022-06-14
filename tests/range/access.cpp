// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/range/access/all.hpp>
#include <iterator>

namespace test {

struct custom_array
{
    int arr[4];
};

constexpr auto begin(custom_array const& a) noexcept
{
    return std::begin(a.arr);
}

constexpr auto end(custom_array const& a) noexcept
{
    return std::end(a.arr);
}

constexpr auto data(custom_array const& a) noexcept
{
    return std::data(a.arr);
}

constexpr auto size(custom_array const& a) noexcept
{
    return std::size(a.arr);
}

} // namespace test

constexpr test::custom_array arr{1, 2, 3, 4};
static_assert(1 == *ac::ranges::begin(arr));
static_assert(4 == *(ac::ranges::end(arr) - 1));

static_assert(1 == *ac::ranges::data(arr));

static_assert(4 == ac::ranges::size(arr));
