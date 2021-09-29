#include <actl/container/array/semi_static_array_selector.hpp>
#include <actl/container/conversion/from_tuple.hpp>
#include <actl/container/conversion/integral_constant.hpp>
#include <actl/container/equal_sequence/range_tuple.hpp>

template <class T, T... Values>
using ssa = ac::semi_static_array_t<T, Values...>;

inline constexpr size_t dyn = ac::dynamic_extent<>;

/* equal_sequence */

/* sequences with 1 element */
static_assert(ac::equal_sequence(ssa<size_t, 4>{}, ssa<size_t, dyn>{4}));
static_assert(!ac::equal_sequence(ssa<size_t, 4>{}, ssa<size_t, dyn>{5}));
static_assert(!ac::equal_sequence(ssa<size_t, 4>{}, ssa<size_t, 5>{}));
static_assert(!ac::equal_sequence(ssa<size_t, dyn>{4}, ssa<size_t, dyn>{5}));
/* sequences with 4 elements */
using ssaX4X2 = ssa<int, -1, 4, -1, 2>;
static_assert(ac::equal_sequence(ssaX4X2{5, 3}, ssa<int, 5, 4, 3, 2>{}));
static_assert(!ac::equal_sequence(ssaX4X2{5, 3}, ssa<int, 6, 4, 3, 2>{}));
static_assert(
    ac::equal_sequence(ssaX4X2{5, 3}, ssa<int, -1, -1, -1, -1>{5, 4, 3, 2}));
static_assert(
    !ac::equal_sequence(ssaX4X2{5, 3}, ssa<int, -1, -1, -1, -1>{5, 2, 3, 2}));
static_assert(ac::equal_sequence(ssaX4X2{5, 3}, ssa<int, 5, -1, 3, -1>{4, 2}));
static_assert(!ac::equal_sequence(ssaX4X2{5, 3}, ssa<int, 5, -1, 3, -1>{4, 3}));
/* sequences with not matching number of elements */
static_assert(!ac::equal_sequence(ssa<size_t>{}, ssa<size_t, dyn>{4}));
static_assert(!ac::equal_sequence(ssaX4X2{5, 3}, ssa<int, 5, 4, 3>{}));
static_assert(
    !ac::equal_sequence(ssaX4X2{5, 3}, ssa<int, -1, -1, -1>{5, 4, 3}));
/* equal_sequence is always noexcept */
static_assert(
    noexcept(ac::equal_sequence(ssa<size_t, 4>{}, ssa<size_t, dyn>{5})));
static_assert(
    noexcept(ac::equal_sequence(ssaX4X2{5, 3}, ssa<int, 6, 4, 3, 2>{})));
static_assert(
    noexcept(ac::equal_sequence(ssaX4X2{5, 3}, ssa<int, -1, -1, -1>{5, 4, 3})));
