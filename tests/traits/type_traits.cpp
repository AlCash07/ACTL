/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/test.hpp>
#include <actl/traits/type_traits.hpp>

using namespace ac;

TEST("add_const_if") {
    ASSERT_TRUE(std::is_same_v<int, add_const_if_t<false, int>>);
    ASSERT_TRUE(std::is_same_v<const int, add_const_if_t<true, int>>);
}

struct functor {
    void operator()(int, double) {}
};

TEST("is_invocable") {
    ASSERT_TRUE(is_invocable_v<functor, int, double>);
    ASSERT_TRUE(is_invocable_v<functor, char, int>);
    ASSERT_FALSE(is_invocable_v<functor, int, const char*>);
    ASSERT_FALSE(is_invocable_v<functor, int, double, int>);
}

TEST("is_non_const_reference") {
    ASSERT_FALSE(is_non_const_reference_v<int>);
    ASSERT_TRUE(is_non_const_reference_v<int&>);
    ASSERT_FALSE(is_non_const_reference_v<const int&>);
}

TEST("is_signed_int") {
    ASSERT_FALSE(is_signed_int_v<bool>);
    ASSERT_TRUE(is_signed_int_v<char>);
    ASSERT_TRUE(is_signed_int_v<long long>);
    ASSERT_FALSE(is_signed_int_v<unsigned long long>);
    ASSERT_FALSE(is_signed_int_v<float>);
}

TEST("is_unsigned_int") {
    ASSERT_TRUE(is_unsigned_int_v<bool>);
    ASSERT_TRUE(is_unsigned_int_v<unsigned char>);
    ASSERT_TRUE(is_unsigned_int_v<unsigned long long>);
    ASSERT_FALSE(is_unsigned_int_v<int>);
    ASSERT_FALSE(is_unsigned_int_v<float>);
}

TEST("is_static_castable") {
    ASSERT_TRUE(is_static_castable_v<double, int>);
    ASSERT_FALSE(is_static_castable_v<double, const char*>);
}

TEST("remove_cvref") { ASSERT_TRUE(std::is_same_v<int, remove_cvref_t<volatile const int&>>); }

TEST("remove_rvalue_reference") {
    ASSERT_TRUE(std::is_same_v<int, remove_rvalue_reference_t<int>>);
    ASSERT_TRUE(std::is_same_v<int&, remove_rvalue_reference_t<int&>>);
    ASSERT_TRUE(std::is_same_v<int, remove_rvalue_reference_t<int&&>>);
}
