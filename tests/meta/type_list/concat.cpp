// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/meta/type_list/concat.hpp>
#include <type_traits>

// Testing concatenation of 0 to 3 type lists,
// because each of them could be a special case.
static_assert(std::is_same_v<ac::type_list<>, ac::concat_t<>>);
static_assert(std::is_same_v<
              ac::type_list<int, long>,
              ac::concat_t<ac::type_list<int, long>>>);
static_assert(std::is_same_v<
              ac::type_list<void, int, long>,
              ac::concat_t<ac::type_list<void>, ac::type_list<int, long>>>);
static_assert(std::is_same_v<
              ac::type_list<int, long, float, double, void>,
              ac::concat_t<
                  ac::type_list<int, long>,
                  ac::type_list<float, double>,
                  ac::type_list<void>>>);
