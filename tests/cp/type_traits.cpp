/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/cp/type_traits.hpp>
#include <actl/test.hpp>

#include <array>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

TEST("cp::is_container") {
    ASSERT_TRUE(ac::is_container_v<std::vector<int>>);
    ASSERT_TRUE(ac::is_container_v<std::array<int, 10>>);
    ASSERT_TRUE(ac::is_container_v<int[10]>);
}

TEST("cp::is_associative_container") {
    ASSERT_TRUE(ac::is_associative_container_v<std::set<int>>);
    ASSERT_TRUE(ac::is_associative_container_v<std::set<int>>);
    ASSERT_TRUE(ac::is_associative_container_v<std::map<int, int>>);
    ASSERT_FALSE(ac::is_associative_container_v<std::vector<int>>);
    ASSERT_FALSE(ac::is_associative_container_v<std::array<int, 10>>);
    ASSERT_FALSE(ac::is_associative_container_v<int[10]>);
}

TEST("cp::is_sequence_container") {
    ASSERT_TRUE(ac::is_sequence_container_v<std::array<int, 10>>);
    ASSERT_TRUE(ac::is_sequence_container_v<std::vector<int>>);
    ASSERT_FALSE(ac::is_sequence_container_v<std::set<int>>);
    ASSERT_FALSE(ac::is_sequence_container_v<std::set<int>>);
    ASSERT_FALSE(ac::is_sequence_container_v<std::map<int, int>>);
}

TEST("cp::is_map") {
    ASSERT_FALSE(ac::is_map_v<std::set<int>>);
    ASSERT_TRUE(ac::is_map_v<std::map<int, int>>);
}

TEST("cp::is_set") {
    ASSERT_TRUE(ac::is_set_v<std::set<int>>);
    ASSERT_FALSE(ac::is_set_v<std::map<int, int>>);
}

TEST("cp::is_array") {
    ASSERT_TRUE(ac::is_array_v<int[10]>);
    ASSERT_TRUE(ac::is_array_v<std::array<int, 10>>);
    ASSERT_FALSE(ac::is_array_v<std::vector<int>>);
}

TEST("cp::is_string") {
    ASSERT_TRUE(ac::is_string_v<std::string>);
    ASSERT_TRUE(ac::is_string_v<const char[10]>);
    ASSERT_TRUE(ac::is_string_v<const char[]>);
    ASSERT_TRUE(ac::is_string_v<char[]>);
    ASSERT_TRUE(ac::is_string_v<const char*>);
    ASSERT_TRUE(ac::is_string_v<char*>);
}

TEST("cp::type_kind_of") {
    using namespace ac;
    ASSERT_EQUAL(type_kind::boolean, type_kind_of<bool>);
    ASSERT_EQUAL(type_kind::signed_integer, type_kind_of<int>);
    ASSERT_EQUAL(type_kind::unsigned_integer, type_kind_of<unsigned>);
    ASSERT_EQUAL(type_kind::floating_point, type_kind_of<float>);
    ASSERT_EQUAL(type_kind::pair, type_kind_of<std::pair<bool, int>>);
    ASSERT_EQUAL(type_kind::tuple, type_kind_of<std::tuple<bool>>);
    ASSERT_EQUAL(type_kind::string, type_kind_of<char[]>);
    ASSERT_EQUAL(type_kind::string, type_kind_of<const std::string&>);
    ASSERT_EQUAL(type_kind::set, type_kind_of<std::unordered_set<int>>);
    ASSERT_EQUAL(type_kind::map, type_kind_of<std::unordered_map<int, int>>);
    ASSERT_EQUAL(type_kind::sequence_container, type_kind_of<std::vector<std::string>>);
    ASSERT_EQUAL(type_kind::array, type_kind_of<int[42]>);
}
