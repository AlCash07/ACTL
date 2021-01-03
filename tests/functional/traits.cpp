// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/traits.hpp>
#include <functional>

bool ok();

TEST_CASE("0_arity") {
    using F = decltype(ok);
    CHECK(0ul == arity_v<F>);
    CHECK(std::is_same_v<bool, return_type_t<F>>);
}

int cmp(const void*, const std::string&);

TEST_CASE("function") {
    using F = decltype(cmp);
    CHECK(2ul == arity_v<F>);
    CHECK(std::is_same_v<int, return_type_t<F>>);
    CHECK(std::is_same_v<const void*, argument_type_t<F, 0>>);
    CHECK(std::is_same_v<const std::string&, argument_type_t<F, 1>>);
}

struct S {
    void f1(bool&&);

    double f0() const;

    bool operator()(int, S) const;
};

TEST_CASE("member_function") {
    using F = decltype(&S::f1);
    CHECK(2ul == arity_v<F>);
    CHECK(std::is_same_v<void, return_type_t<F>>);
    CHECK(std::is_same_v<S&, argument_type_t<F, 0>>);
    CHECK(std::is_same_v<bool&&, argument_type_t<F, 1>>);
}

TEST_CASE("const_member_function") {
    using F = decltype(&S::f0);
    CHECK(1ul == arity_v<F>);
    CHECK(std::is_same_v<double, return_type_t<F>>);
    CHECK(std::is_same_v<const S&, argument_type_t<F, 0>>);
}

TEST_CASE("functor") {
    CHECK(2ul == arity_v<S>);
    CHECK(std::is_same_v<bool, return_type_t<S>>);
    CHECK(std::is_same_v<int, argument_type_t<S, 0>>);
    CHECK(std::is_same_v<S, argument_type_t<S, 1>>);
}

TEST_CASE("std::function") {
    using F = std::function<char(char&)>;
    CHECK(1ul == arity_v<F>);
    CHECK(std::is_same_v<char, return_type_t<F>>);
    CHECK(std::is_same_v<char&, argument_type_t<F, 0>>);
}
