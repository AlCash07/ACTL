// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/arithmetic/sub.hpp>
#include <actl/functional/scalar/common/cast.hpp>
#include <actl/functional/scalar/comparison/greater.hpp>
#include <actl/functional/scalar/comparison/less.hpp>

namespace ac {

struct cmp3way_t : comparison_operation<cmp3way_t> {
    static constexpr auto formula = cast<int>(greater) - cast<int>(less);
};
constexpr cmp3way_t cmp3way;

}  // namespace ac
