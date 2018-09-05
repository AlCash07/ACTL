/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/container_traits.hpp>
#include <deque>

namespace ac {

template <class T, class A>
struct container_traits<std::deque<T, A>> {
    using category = random_access_container_tag;

    template <class T1, class A1 = rebind_allocator_t<A, T1>>
    using rebind = std::deque<T1, A1>;
};

}  // namespace ac
