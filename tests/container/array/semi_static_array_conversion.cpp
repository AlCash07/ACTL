// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/semi_static_array_selector.hpp>
#include <actl/container/conversion/from_tuple.hpp>
#include <actl/container/conversion/to_integral_constant.hpp>
#include <actl/container/conversion/to_tuple.hpp>

template <class T, T... Values>
using ssa = ac::semi_static_array_t<T, Values...>;

inline constexpr size_t dyn = ac::dynamic_extent<>;

using ssa542_t = ssa<size_t, 5, 4, 2>;
constexpr auto ssa542_v = ssa542_t{};
using ssa5X2_t = ssa<size_t, 5, dyn, 2>;
constexpr auto ssa5X2_v = ssa5X2_t{4};
using ssaXX2_t = ssa<size_t, dyn, dyn, 2>;
constexpr auto ssaXX2_v = ssaXX2_t{5, 4};
using ssaXXX_t = ssa<size_t, dyn, dyn, dyn>;
constexpr auto ssaXXX_v = ssaXXX_t{5, 4, 2};

static_assert(ssa542_v == ac::convert_to<ssa542_t>(ssaXX2_v));
static_assert(ssa542_v == ac::convert_to<ssa542_t>(ssaXXX_v));
static_assert(ssa5X2_v == ac::convert_to<ssa5X2_t>(ssa542_v));
static_assert(ssa5X2_v == ac::convert_to<ssa5X2_t>(ssa5X2_v));
static_assert(ssaXX2_v == ac::convert_to<ssaXX2_t>(ssa5X2_v));
static_assert(ssaXX2_v == ac::convert_to<ssaXX2_t>(ssaXXX_v));
static_assert(
    ac::detail::equal_array(ssaXXX_v, ac::convert_to<ssaXXX_t>(ssa542_v)));
static_assert(
    ac::detail::equal_array(ssaXXX_v, ac::convert_to<ssaXXX_t>(ssa5X2_v)));

/* not matching static values */
static_assert(!ac::can_convert_to_v<ssa<size_t, 5, 4>, ssa<size_t, 4, 4>>);
static_assert(!ac::can_convert_to_v<ssa<size_t, 5, dyn>, ssa<size_t, 4, dyn>>);
static_assert(!ac::can_convert_to_v<ssa<size_t, dyn, 3>, ssa<size_t, dyn, 4>>);
/* not matching number of elements */
static_assert(!ac::can_convert_to_v<ssa542_t, ssa<size_t, 5, 4>>);
static_assert(!ac::can_convert_to_v<ssa<size_t, 5, dyn>, ssa5X2_t>);
static_assert(!ac::can_convert_to_v<ssaXXX_t, ssa<size_t, 5, dyn>>);
