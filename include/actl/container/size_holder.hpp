/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/util/static_size.hpp>

namespace ac {

template <index N>
class size_holder {
public:
    static_assert(N >= 0);

    constexpr size_holder(index n [[maybe_unused]]) { ACTL_ASSERT(n == N); }

    static constexpr index size() { return N; }
};

template <>
class size_holder<dynamic_size> {
public:
    constexpr size_holder(index n) : size_{n} { ACTL_ASSERT(n >= 0); }

    constexpr index size() const { return size_; }

private:
    index size_;
};

}  // namespace ac
