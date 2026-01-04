// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/none.hpp>
#include <actl/range/iterator/traits/category.hpp>
#include <actl/range/traits/associated_types.hpp>
#include <actl_test/core/regular.hpp>
#include <actl_test/operation/relational_operators.hpp>
#include <iterator>

namespace ac {

// https://en.cppreference.com/w/cpp/iterator/input_or_output_iterator
template<typename Iterator, typename Category>
void test_iterator(Iterator iter, Category) {
    static_assert(!std::is_const_v<std::iter_value_t<Iterator>>);
    static_assert(std::is_same_v<Iterator&, decltype(++iter)>);
    CHECK(std::addressof(++iter) == std::addressof(iter));
    iter++;
}

// https://en.cppreference.com/w/cpp/iterator/input_iterator
template<typename Iterator>
void test_iterator(Iterator iter, std::input_iterator_tag) {
    test_iterator(iter, none{});
    static_assert(std::is_base_of_v<
                  std::input_iterator_tag,
                  iter_category_t<Iterator>>);
    static_assert(std::is_same_v<
                  std::iter_reference_t<Iterator>,
                  decltype(*iter)>);
}

// https://en.cppreference.com/w/cpp/iterator/forward_iterator
template<typename Iterator>
void test_iterator(Iterator iter, std::forward_iterator_tag) {
    test_iterator(iter, std::input_iterator_tag{});
    static_assert(std::is_base_of_v<
                  std::forward_iterator_tag,
                  iter_category_t<Iterator>>);
    if constexpr (std::equality_comparable<std::iter_reference_t<Iterator>>)
        CHECK(*iter == *iter);
    Iterator prev = iter;
    ++iter;
    test_regular(iter, prev);
    static_assert(std::is_same_v<Iterator, decltype(iter++)>);
    iter = prev;
    CHECK(iter++ == prev);
    iter = prev;
    CHECK(((void)iter++, iter) == ++prev);
}

// https://en.cppreference.com/w/cpp/iterator/bidirectional_iterator
template<typename Iterator>
void test_iterator(Iterator iter, std::bidirectional_iterator_tag) {
    test_iterator(iter, std::forward_iterator_tag{});
    static_assert(std::is_base_of_v<
                  std::bidirectional_iterator_tag,
                  iter_category_t<Iterator>>);
    static_assert(std::is_same_v<Iterator&, decltype(--iter)>);
    static_assert(std::is_same_v<Iterator, decltype(iter--)>);
    Iterator prev = iter;
    ++iter;
    Iterator next = iter;
    CHECK(std::addressof(--iter) == std::addressof(iter));
    iter = next;
    CHECK(iter-- == next);
    CHECK(--(++iter) == prev);
    iter = next;
    CHECK(++(--iter) == next);
    CHECK(((void)iter--, iter) == --next);
}

// https://en.cppreference.com/w/cpp/iterator/random_access_iterator
template<typename Iterator>
void test_random_access_iterator(Iterator i, Iterator j) {
    REQUIRE(i < j);
    test_iterator(i, std::bidirectional_iterator_tag{});
    static_assert(std::is_base_of_v<
                  std::random_access_iterator_tag,
                  iter_category_t<Iterator>>);
    test_relational_operators_for_equal_values(i, i);
    test_relational_operators_for_different_values(i, j);
    static_assert(std::is_same_v<
                  std::iter_difference_t<Iterator>,
                  decltype(j - i)>);
    std::iter_difference_t<Iterator> n = j - i;
    Iterator const i_const = i;
    static_assert(std::is_same_v<Iterator&, decltype(i += n)>);
    static_assert(std::is_same_v<Iterator, decltype(i_const + n)>);
    static_assert(std::is_same_v<Iterator, decltype(n + i_const)>);
    static_assert(std::is_same_v<Iterator&, decltype(i -= n)>);
    static_assert(std::is_same_v<Iterator, decltype(i_const - n)>);
    static_assert(std::is_same_v<
                  std::iter_reference_t<Iterator>,
                  decltype(i_const[n])>);
    Iterator init_i = i;
    CHECK((i += n) == j);
    i = init_i;
    CHECK(std::addressof(i += n) == std::addressof(i));
    i = init_i;
    CHECK((init_i + n) == (i += n));
    i = init_i;
    CHECK((i + n) == (n + i));
    for (std::iter_difference_t<Iterator> x = 0; x <= n && x < 8; ++x)
        CHECK((i + n) == ((i + x) + (n - x)));
    CHECK(i + 0 == i);
    Iterator init_j = j;
    CHECK(i + (n - 1) == --j);
    j = init_j;
    CHECK((j += -n) == i);
    j = init_j;
    CHECK((j -= n) == i);
    j = init_j;
    CHECK(std::addressof(j -= n) == std::addressof(j));
}

} // namespace ac
