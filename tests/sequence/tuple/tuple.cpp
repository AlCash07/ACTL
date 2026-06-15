// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/numeric/constant_literals.hpp>
#include <actl/platform/compiler.hpp>
#include <actl/sequence/tuple/tuple.hpp>
#include <tuple>

using namespace ac::constant_literals;

struct empty {};
static_assert(ac::EmptyTrivial<empty>);

/* tuple construction is done via aggregate initilization */
constexpr ac::tuple<int, int> tuple_ii{4, 2};
constexpr ac::tuple<int, empty> tuple_ie{4, {}};

/* elements are accessed by operator[] */
// TODO: test overloads with all different qualifiers.
static_assert(4 == tuple_ii[0_c]);
static_assert(2 == tuple_ii[1_c]);
static_assert(4 == tuple_ie[0_c]);
static_assert(std::is_same_v<empty, decltype(tuple_ie[1_c])>);

/* empty and trivial element properties are preserved */
static_assert(ac::EmptyTrivial<ac::tuple<empty>>);
static_assert(ac::EmptyTrivial<ac::tuple<empty, empty>>);
static_assert(ac::EmptyTrivial<ac::tuple<ac::tuple<empty>>>);
static_assert(ac::EmptyTrivial<ac::tuple<ac::tuple<empty>, ac::tuple<empty>>>);
static_assert(std::is_trivially_move_assignable_v<ac::tuple<empty>>);

// std::tuple is broken
static_assert(!std::is_trivially_move_assignable_v<std::tuple<empty>>);

/* empty elements introduce no storage overhead */
static_assert(sizeof(int) == sizeof(ac::tuple<int>));
static_assert(sizeof(int) == sizeof(ac::tuple<int, empty>));
static_assert(sizeof(int) == sizeof(ac::tuple<int, empty, empty>));
static_assert(sizeof(int) * 2 == sizeof(ac::tuple<int, empty, empty, int>));

// std::tuple size is broken on MSVC
#if AC_COMPILER_MSVC()
static_assert(!std::is_empty_v<std::tuple<empty>>);
static_assert(sizeof(int) < sizeof(std::tuple<int, empty, empty>));
#endif
