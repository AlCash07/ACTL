// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/numeric/arithmetic/additive/add.hpp>
#include <actl/operation/operation/operation_base.hpp>
#include <memory> // for std::unique_ptr
#include "test.hpp"

struct non_trivial_operation : ac::operation_base<non_trivial_operation> {
    std::unique_ptr<int> member = nullptr;

    int evaluate(const std::unique_ptr<int>& l, int r) const {
        return *member + *l + r;
    }
};

TEST_CASE("operation_base without ac::inout arguments creates an expression") {
    /* r-values are always stored by value. */
    auto expr_r = non_trivial_operation{}(std::unique_ptr<int>{}, 2);
    using type_r =
        ac::expression<non_trivial_operation, std::unique_ptr<int>, int>;
    static_assert(std::is_same_v<decltype(expr_r), type_r>);
    /* l-values are stored by value only if cheap to copy,
       and are always treated as const. */
    non_trivial_operation op{};
    std::unique_ptr<int> arg0;
    int arg1 = 1;
    auto expr_l = op(arg0, arg1);
    auto expr_lc = std::as_const(op)(std::as_const(arg0), std::as_const(arg1));
    using type_l = ac::expression<
        non_trivial_operation const&,
        std::unique_ptr<int> const&,
        int>;
    static_assert(std::is_same_v<decltype(expr_l), type_l>);
    static_assert(std::is_same_v<decltype(expr_lc), type_l>);
}

TEST_CASE("operation_base with ac::inout argument is immediately evaluated") {
    int x = 2;
    int y = 3;
    /* Any compatible argument can be ac::inout. */
    /* Resulting reference is returned. */
    REQUIRE(5 == ac::add(ac::inout{x}, y));
    REQUIRE(5 == x);
    REQUIRE(3 == y); // other argument is unchanged
    REQUIRE(8 == ac::add(x, ac::inout{y}));
    REQUIRE(8 == y);
    REQUIRE(5 == x); // other argument is unchanged
    /* The same result can be achieved by using
       assignment operators on ac::inout. */
    REQUIRE(13 == (ac::inout{x} += y));
    REQUIRE(13 == x);
    REQUIRE(8 == y); // other argument is unchanged
}
