/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/test.hpp>
#include <actl/traits/function_traits.hpp>
#include <functional>

using namespace ac;

inline bool ok();

TEST("0_arity") {
    using F = decltype(ok);
    ASSERT_EQUAL(0u, arity_v<F>);
    ASSERT_TRUE(std::is_same_v<bool, return_type_t<F>>);
}

inline int cmp(const void*, const std::string&);

TEST("function") {
    using F = decltype(cmp);
    ASSERT_EQUAL(2u, arity_v<F>);
    ASSERT_TRUE(std::is_same_v<int, return_type_t<F>>);
    ASSERT_TRUE(std::is_same_v<const void*, argument_type_t<F, 0>>);
    ASSERT_TRUE(std::is_same_v<const std::string&, argument_type_t<F, 1>>);
}

struct S {
    void f1(bool&&);

    double f0() const;

    bool operator()(int, S) const;
};

TEST("member_function") {
    using F = decltype(&S::f1);
    ASSERT_EQUAL(2u, arity_v<F>);
    ASSERT_TRUE(std::is_same_v<void, return_type_t<F>>);
    ASSERT_TRUE(std::is_same_v<S&, argument_type_t<F, 0>>);
    ASSERT_TRUE(std::is_same_v<bool&&, argument_type_t<F, 1>>);
}

TEST("const_member_function") {
    using F = decltype(&S::f0);
    ASSERT_EQUAL(1u, arity_v<F>);
    ASSERT_TRUE(std::is_same_v<double, return_type_t<F>>);
    ASSERT_TRUE(std::is_same_v<const S&, argument_type_t<F, 0>>);
}

TEST("functor") {
    ASSERT_EQUAL(2u, arity_v<S>);
    ASSERT_TRUE(std::is_same_v<bool, return_type_t<S>>);
    ASSERT_TRUE(std::is_same_v<int, argument_type_t<S, 0>>);
    ASSERT_TRUE(std::is_same_v<S, argument_type_t<S, 1>>);
}

TEST("std::function") {
    using F = std::function<char(char&)>;
    ASSERT_EQUAL(1u, arity_v<F>);
    ASSERT_TRUE(std::is_same_v<char, return_type_t<F>>);
    ASSERT_TRUE(std::is_same_v<char&, argument_type_t<F, 0>>);
}
