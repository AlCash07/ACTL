// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/function.hpp>
#include <actl/utility/index.hpp>

namespace ac::io {

struct flush_t
{
    bool operator()(Device auto& od) const
    {
        od.flush();
        return true;
    }
};
inline constexpr flush_t flush;

} // namespace ac::io
