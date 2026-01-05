// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/meta/constant.hpp>
#include <actl/numeric/arithmetic/additive/add.hpp>
#include <actl/operation/operation/operation_base.hpp>
#include "core/int_move_copy.hpp"
#include "test.hpp"

template<typename Int>
struct sum : ac::operation_base<sum<Int>> {
    Int member{};

    int evaluate(auto const& l, int r) const {
        return member.value + l.value + r;
    }
};

template<typename IntOp, typename IntArg, bool IsNoexcept>
constexpr void test_rvalue_arguments() {
    constexpr auto expr = sum<IntOp>{{}, 2}(IntArg{3}, 5);
    /* Result of applying an operation is always an expression. */
    /* If r-values are passed, expression stores them by value. */
    using expr_type = ac::expression<sum<IntOp>, IntArg, int>;
    static_assert(std::is_same_v<expr_type const, decltype(expr)>);
    /* Correct values are stored. */
    static_assert(2 == expr.operation.member.value);
    static_assert(3 == get<0>(expr.arguments).value);
    static_assert(5 == get<1>(expr.arguments));
    /* `noexcept` specification is deduced from the arguments' moves. */
    static_assert(IsNoexcept == noexcept(sum<IntOp>{{}, 2}(IntArg{3}, 5)));
}

template<typename IntOp, typename IntArg>
void test_lvalue_arguments() {
    sum<IntOp> op{{}, 2};
    IntArg arg0{3};
    int arg1{5};
    auto expr = op(arg0, arg1);
    /* Result of applying an operation is always an expression. */
    /* If l-values are passed, expression always treats them as const,
     * and stores by value only if cheap to copy. */
    using expr_type = ac::expression<sum<IntOp> const&, IntArg const&, int>;
    static_assert(std::is_same_v<expr_type, decltype(expr)>);
    /* Correct values are stored. */
    CHECK(2 == expr.operation.member.value);
    CHECK(3 == get<0>(expr.arguments).value);
    CHECK(5 == get<1>(expr.arguments));
    /* Expression is always `noexcept` here,
     * because it stored non-trivial types by value. */
    static_assert(noexcept(op(arg0, arg1)));
}

TEST_CASE("operation_base without ac::inout arguments creates an expression") {
    /* expression created from r-values */
    test_rvalue_arguments<int_mo, int_mo, true>();
    test_rvalue_arguments<int_mo, int_cmt, true>();
    test_rvalue_arguments<int_cmt, int_momt, false>();
    test_rvalue_arguments<int_mo, int_momt, false>();
    test_rvalue_arguments<int_momt, int_mo, false>();
    test_rvalue_arguments<int_momt, int_momt, false>();
    /* expression created from l-values */
    test_lvalue_arguments<int_mo, int_cmt>();
    test_lvalue_arguments<int_mo const, int_cmt const>();
    test_lvalue_arguments<int_momt, int_mo>();
    test_lvalue_arguments<int_momt const, int_mo const>();
    /* expression size is the sum of its components */
    static_assert(3 * sizeof(int) == sizeof(sum<int_mo>{{}, 0}(int_mo{1}, 2)));
    /* empty operation */
    static_assert(2 * sizeof(int) == sizeof(ac::add(1, 2)));
    /* empty argument */
    static_assert(
        2 * sizeof(int) == sizeof(sum<int_mo>{{}, 0}(int_mo{1}, 2_c))
    );
    /* empty operation and arguments */
    // TODO: currently fails because of std::tuple.
    // static_assert(std::is_empty_v<decltype(ac::add_integer(1_c, 2_c))>);
    // TODO: currently fails because of std::tuple.
    // static_assert(std::is_trivially_copyable_v<decltype(ac::add(1, 2))>);
}
