#pragma once

#include <actl/cp/random/rng_decorator.hpp>
#include <actl/cp/random/xoroshiro128cplus.hpp>

namespace cp {

using default_random = rng_decorator<xoroshiro128cplus>;

static default_random random;

}  // namespace cp
