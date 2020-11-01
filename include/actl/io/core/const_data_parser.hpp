/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/span.hpp>
#include <actl/range/algorithm.hpp>

namespace ac::io {

struct const_data_parser {
    cspan<char> pending;

    index parse(cspan<char> s) {
        auto matching = mismatch(pending, s).second - s.begin();
        pending = pending.subspan(matching);
        return matching;
    }

    bool ready() const { return pending.empty(); }
};

}  // namespace ac::io
