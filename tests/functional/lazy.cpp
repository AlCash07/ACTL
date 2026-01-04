// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/lazy.hpp>
#include <actl_test/core/lifetime_counter.hpp>
#include <map>
#include <optional>
#include "test.hpp"

namespace ac {

namespace {

size_t invocations_count = 0;

int counts_invocations() {
    ++invocations_count;
    return 3;
}

} // namespace

TEST_CASE("lazy") {
    SECTION(
        "try_emplace calls the function only if the element doesn't already "
        "exist"
    ) {
        std::map<int, int> map;
        map.try_emplace(0, lazy{counts_invocations});
        CHECK(invocations_count == 1); // new element
        map.try_emplace(0, lazy{counts_invocations});
        CHECK(invocations_count == 1); // existing element
        map.try_emplace(1, lazy{counts_invocations});
        CHECK(invocations_count == 2); // new element
    }

    struct Tag {};
    auto& counts = lifetime_counter<Tag>::counts;
    auto make_counter = [] {
        return lifetime_counter<Tag>{};
    };
    SECTION("move constructor is called after a usual function call") {
        std::optional<lifetime_counter<Tag>> optional{make_counter()};
        CHECK(counts.default_constructor_count == 1);
        CHECK(counts.move_constructor_count == 1);
        CHECK(counts.copy_constructor_count == 0);
    }
    SECTION("move constructor isn't called with lazy") {
        counts = {};
        std::optional<lifetime_counter<Tag>> optional{lazy{make_counter}};
        CHECK(counts.default_constructor_count == 1);
        CHECK(counts.move_constructor_count == 0);
        CHECK(counts.copy_constructor_count == 0);
    }
}

} // namespace ac
