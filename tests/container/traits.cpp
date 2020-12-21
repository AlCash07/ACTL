// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/container/traits.hpp>
#include <actl/std/vector.hpp>
#include <memory>

TEST_CASE("rebind_container") {
    CHECK(std::is_same_v<dummy_container, rebind_container_t<none, int>>);
    CHECK(std::is_same_v<dummy_container, rebind_container_t<dummy_container, int>>);
    CHECK(std::is_same_v<dummy_container, rebind_container_t<std::vector<int>, none>>);
    CHECK(std::is_same_v<std::vector<int>, rebind_container_t<std::vector<none>, int>>);
}

TEST_CASE("is_smart_pointer") {
    CHECK(is_smart_pointer<std::unique_ptr<float>>::value);
    CHECK(is_smart_pointer<const std::unique_ptr<std::vector<int>>>::value);
    CHECK(is_smart_pointer<std::shared_ptr<float>>::value);
    CHECK_FALSE(is_smart_pointer<std::vector<float>>::value);
}
