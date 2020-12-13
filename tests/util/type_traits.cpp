// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/std/vector.hpp>
#include <actl/test.hpp>
#include <actl/util/type_traits.hpp>

TEST("add_const_if") {
    static_assert(std::is_same_v<int, add_const_if_t<false, int>>);
    static_assert(std::is_same_v<const int, add_const_if_t<true, int>>);
}

TEST("remove_cvref") { static_assert(std::is_same_v<int, remove_cvref_t<volatile const int&>>); }

TEST("is_one_of_v") {
    static_assert(is_one_of_v<int, int>);
    static_assert(is_one_of_v<int, double, int, float>);
    static_assert(!is_one_of_v<int, const int, int&, int*>);
}

TEST("are_same") {
    static_assert(are_same_v<int>);
    static_assert(are_same_v<int, int>);
    static_assert(are_same_v<int, int, int>);
    static_assert(are_same_v<int, int, int, int>);
    static_assert(!are_same_v<int*, int, int, int>);
    static_assert(!are_same_v<int, int, int&, int>);
    static_assert(!are_same_v<int, int, int, double>);
}

class public_derived : public std::vector<int> {};
class private_derived : private std::vector<int> {};

TEST("is_template_base_of") {
    static_assert(is_template_base_of_v<std::vector, std::vector<double>>);
    static_assert(is_template_base_of_v<std::vector, public_derived>);
    static_assert(is_template_base_of_v<std::vector, private_derived>);
    static_assert(!is_template_base_of_v<std::vector, std::pair<int, int>>);
    static_assert(!is_template_base_of_v<std::vector, int>);
}

TEST("nth_type") {
    static_assert(std::is_same_v<int, nth_t<0, int, float>>);
    static_assert(std::is_same_v<float, nth_t<1, int, float>>);
}

TEST("value_t") {
    static_assert(std::is_same_v<const int, value_t<std::vector<const int>>>);
    static_assert(std::is_same_v<const int, value_t<const int[8]>>);
    static_assert(std::is_same_v<int, value_t<const int* const>>);
}

TEST("reference_t") {
    static_assert(std::is_same_v<int&, reference_t<std::vector<int>>>);
    static_assert(std::is_same_v<const int&, reference_t<const std::vector<int>>>);
    static_assert(std::is_same_v<int&, reference_t<int[8]>>);
    static_assert(std::is_same_v<const int&, reference_t<const int* const>>);
}

TEST("iterator_t") {
    using VI = std::vector<int>;
    static_assert(std::is_same_v<VI::iterator, iterator_t<VI>>);
    static_assert(std::is_same_v<VI::const_iterator, iterator_t<const VI>>);
    static_assert(std::is_same_v<int*, iterator_t<int[8]>>);
    static_assert(std::is_same_v<const int*, iterator_t<int const[8]>>);
}
