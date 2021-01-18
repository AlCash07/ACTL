// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/utility/index.hpp>

namespace ac::io {

struct flush_t {
    template <class Device>
    index operator()(Device& od) const {
        od.flush();
        return 0;
    }
};
constexpr flush_t flush;

}  // namespace ac::io
