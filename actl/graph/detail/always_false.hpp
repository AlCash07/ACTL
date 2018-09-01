/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

namespace ac {

struct always_false {
    template <class T>
    constexpr bool operator()(T) const {
        return false;
    }
};

}  // namespace ac