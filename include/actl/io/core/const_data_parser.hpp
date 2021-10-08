// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/algorithm.hpp>
#include <actl/range/span.hpp>

namespace ac::io {

struct const_data_parser
{
    cspan<char> pending;

    size_t parse(cspan<char> s)
    {
        size_t matching =
            static_cast<size_t>(mismatch(pending, s).second - s.begin());
        pending = pending.subspan(matching);
        return matching;
    }

    bool ready() const
    {
        return pending.empty();
    }
};

} // namespace ac::io
