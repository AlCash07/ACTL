// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/traits/concepts.hpp>
#include <deque>
#include <forward_list>
#include <list>
#include <vector>

static_assert(!ac::Range<void>);
static_assert(!ac::ContiguousRange<void>);

static_assert(!ac::Range<int>);
static_assert(!ac::ContiguousRange<int>);

static_assert(ac::ForwardRange<std::forward_list<int>>);
static_assert(!ac::RandomAccessRange<std::forward_list<int>>);

static_assert(ac::ForwardRange<std::list<int>>);
static_assert(ac::BidirectionalRange<std::list<int>>);
static_assert(!ac::RandomAccessRange<std::list<int>>);

static_assert(ac::Range<std::deque<int>>);
static_assert(ac::ForwardRange<std::deque<int>>);
static_assert(ac::BidirectionalRange<std::deque<int>>);
static_assert(ac::RandomAccessRange<std::deque<int>>);
static_assert(!ac::ContiguousRange<std::deque<int>>);

static_assert(ac::Range<std::vector<int>>);
static_assert(ac::ForwardRange<std::vector<int>>);
static_assert(ac::BidirectionalRange<std::vector<int>>);
static_assert(ac::RandomAccessRange<std::vector<int>>);
static_assert(ac::ContiguousRange<std::vector<int>>);

using CArray = int[2];
static_assert(ac::Range<CArray>);
static_assert(ac::Range<CArray const>);
static_assert(ac::RandomAccessRange<CArray>);
static_assert(ac::ContiguousRange<CArray>);

/* Test for non-contiguous range with data() method */
struct array2x2 {
    int arr[2][2];

    auto begin() {
        return ac::ranges::begin(arr);
    }
    auto end() {
        return ac::ranges::end(arr);
    }

    auto data() {
        return &arr[0][0];
    }
};

static_assert(ac::RandomAccessRange<array2x2>);
static_assert(!ac::ContiguousRange<array2x2>);
