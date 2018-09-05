/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/random/rng_decorator.hpp>
#include <actl/random/xoroshiro128cplus.hpp>

namespace ac {

using default_random = rng_decorator<xoroshiro128cplus>;

static default_random random;

}  // namespace ac