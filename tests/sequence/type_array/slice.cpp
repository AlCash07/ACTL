// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/sequence/type_array/slice.hpp>
#include <type_traits>

// The implementation for length 4+ is different,
// so we need a long enough list to test it.
using FullList = ac::type_array<void, int, long, float, double>;

/* length 0 */
static_assert(std::is_same_v<ac::type_array<>, ac::slice_t<FullList, 0, 0>>);
static_assert(std::is_same_v<ac::type_array<>, ac::slice_t<FullList, 2, 2>>);
static_assert(std::is_same_v<ac::type_array<>, ac::slice_t<FullList, 5, 5>>);
/* length 1 */
static_assert(std::
                  is_same_v<ac::type_array<void>, ac::slice_t<FullList, 0, 1>>);
static_assert(std::
                  is_same_v<ac::type_array<long>, ac::slice_t<FullList, 2, 3>>);
static_assert(std::is_same_v<
              ac::type_array<double>,
              ac::slice_t<FullList, 4, 5>>);
/* length 2 */
static_assert(std::is_same_v<
              ac::type_array<void, int>,
              ac::slice_t<FullList, 0, 2>>);
static_assert(std::is_same_v<
              ac::type_array<long, float>,
              ac::slice_t<FullList, 2, 4>>);
static_assert(std::is_same_v<
              ac::type_array<float, double>,
              ac::slice_t<FullList, 3, 5>>);
/* length 3 */
static_assert(std::is_same_v<
              ac::type_array<void, int, long>,
              ac::slice_t<FullList, 0, 3>>);
static_assert(std::is_same_v<
              ac::type_array<int, long, float>,
              ac::slice_t<FullList, 1, 4>>);
static_assert(std::is_same_v<
              ac::type_array<long, float, double>,
              ac::slice_t<FullList, 2, 5>>);
/* length 4 */
static_assert(std::is_same_v<
              ac::type_array<void, int, long, float>,
              ac::slice_t<FullList, 0, 4>>);
static_assert(std::is_same_v<
              ac::type_array<int, long, float, double>,
              ac::slice_t<FullList, 1, 5>>);
/* length 5 */
static_assert(std::is_same_v<FullList, ac::slice_t<FullList, 0, 5>>);

/* prefix and suffix as special cases */
static_assert(std::is_same_v<ac::type_array<void>, ac::prefix_t<FullList, 1>>);
static_assert(std::is_same_v<
              ac::type_array<void, int>,
              ac::prefix_t<FullList, 2>>);
static_assert(std::
                  is_same_v<ac::type_array<double>, ac::suffix_t<FullList, 1>>);
static_assert(std::is_same_v<
              ac::type_array<float, double>,
              ac::suffix_t<FullList, 2>>);
