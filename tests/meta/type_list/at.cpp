// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/type_list/at.hpp>
#include <type_traits>

template<size_t Index>
struct I {};

static_assert(std::is_same_v<int, ac::at_t<ac::type_list<int>, 0>>);

using ShortList = ac::type_list<I<0>, I<1>, I<2>>;

static_assert(std::is_same_v<I<0>, ac::at_t<ShortList, 0>>);
static_assert(std::is_same_v<I<1>, ac::at_t<ShortList, 1>>);
static_assert(std::is_same_v<I<2>, ac::at_t<ShortList, 2>>);

// The implementation for length 8+ is different,
// so we need a long enough list to test it.
using LongList = ac::type_list<
    I<0>,
    I<1>,
    I<2>,
    I<3>,
    I<4>,
    I<5>,
    I<6>,
    I<7>,
    I<8>,
    I<9>,
    I<10>,
    I<11>>;

static_assert(std::is_same_v<I<0>, ac::at_t<LongList, 0>>);
static_assert(std::is_same_v<I<1>, ac::at_t<LongList, 1>>);
static_assert(std::is_same_v<I<2>, ac::at_t<LongList, 2>>);
static_assert(std::is_same_v<I<3>, ac::at_t<LongList, 3>>);
static_assert(std::is_same_v<I<4>, ac::at_t<LongList, 4>>);
static_assert(std::is_same_v<I<5>, ac::at_t<LongList, 5>>);
static_assert(std::is_same_v<I<6>, ac::at_t<LongList, 6>>);
static_assert(std::is_same_v<I<7>, ac::at_t<LongList, 7>>);
static_assert(std::is_same_v<I<8>, ac::at_t<LongList, 8>>);
static_assert(std::is_same_v<I<9>, ac::at_t<LongList, 9>>);
static_assert(std::is_same_v<I<10>, ac::at_t<LongList, 10>>);
static_assert(std::is_same_v<I<11>, ac::at_t<LongList, 11>>);
