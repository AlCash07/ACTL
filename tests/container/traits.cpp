// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/container/traits.hpp>
#include <actl/std/vector.hpp>
#include <actl/test.hpp>
#include <memory>

TEST("rebind_container") {
    ASSERT_TRUE(std::is_same_v<dummy_container, rebind_container_t<none, int>>);
    ASSERT_TRUE(std::is_same_v<dummy_container, rebind_container_t<dummy_container, int>>);
    ASSERT_TRUE(std::is_same_v<dummy_container, rebind_container_t<std::vector<int>, none>>);
    ASSERT_TRUE(std::is_same_v<std::vector<int>, rebind_container_t<std::vector<none>, int>>);
}

TEST("is_smart_pointer") {
    ASSERT_TRUE(is_smart_pointer<std::unique_ptr<float>>::value);
    ASSERT_TRUE(is_smart_pointer<const std::unique_ptr<std::vector<int>>>::value);
    ASSERT_TRUE(is_smart_pointer<std::shared_ptr<float>>::value);
    ASSERT_FALSE(is_smart_pointer<std::vector<float>>::value);
}
