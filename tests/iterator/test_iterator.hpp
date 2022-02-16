#pragma once

#include <actl/iterator/traits.hpp>
#include <actl/meta/dependent.hpp>
#include <actl/meta/type_traits.hpp>
#include <actl/utility/none.hpp>
#include <iterator>
#include "test_regular.hpp"
#include "test_relational_operators.hpp"

namespace ac {

// https://en.cppreference.com/w/cpp/iterator/input_or_output_iterator
template <class Iterator, class Category>
void test_iterator(Iterator iter, Category)
{
    static_assert(!std::is_const_v<value_type_t<Iterator>>);
    static_assert(std::is_same_v<Iterator&, decltype(++iter)>);
    CHECK(std::addressof(++iter) == std::addressof(iter));
}

// https://en.cppreference.com/w/cpp/iterator/input_iterator
template <class Iterator>
void test_iterator(Iterator iter, std::input_iterator_tag)
{
    test_iterator(iter, none{});
    static_assert(std::is_base_of_v<
                  std::input_iterator_tag,
                  iterator_category_t<Iterator>>);
    static_assert(std::is_same_v<reference_t<Iterator>, decltype(*iter)>);
}

// https://en.cppreference.com/w/cpp/iterator/forward_iterator
template <class Iterator>
void test_iterator(Iterator iter, std::forward_iterator_tag)
{
    test_iterator(iter, std::input_iterator_tag{});
    static_assert(std::is_base_of_v<
                  std::forward_iterator_tag,
                  iterator_category_t<Iterator>>);
    if constexpr (is_equality_comparable_v<reference_t<Iterator>>)
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
template <class Iterator>
void test_iterator(Iterator iter, std::bidirectional_iterator_tag)
{
    test_iterator(iter, std::forward_iterator_tag{});
    static_assert(std::is_base_of_v<
                  std::bidirectional_iterator_tag,
                  iterator_category_t<Iterator>>);
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
template <class Iterator>
void test_random_access_iterator(Iterator i, Iterator j)
{
    REQUIRE(i < j);
    test_iterator(i, std::bidirectional_iterator_tag{});
    static_assert(std::is_base_of_v<
                  std::random_access_iterator_tag,
                  iterator_category_t<Iterator>>);
    test_relational_operators_for_equal_values(i, i);
    test_relational_operators_for_different_values(i, j);
    static_assert(std::is_same_v<difference_type_t<Iterator>, decltype(j - i)>);
    difference_type_t<Iterator> n = j - i;
    const Iterator const_i = i;
    static_assert(std::is_same_v<Iterator&, decltype(i += n)>);
    static_assert(std::is_same_v<Iterator, decltype(const_i + n)>);
    static_assert(std::is_same_v<Iterator, decltype(n + const_i)>);
    static_assert(std::is_same_v<Iterator&, decltype(i -= n)>);
    static_assert(std::is_same_v<Iterator, decltype(const_i - n)>);
    static_assert(std::is_same_v<reference_t<Iterator>, decltype(const_i[n])>);
    Iterator init_i = i;
    CHECK((i += n) == j);
    i = init_i;
    CHECK(std::addressof(i += n) == std::addressof(i));
    i = init_i;
    CHECK((i + n) == (i += n));
    i = init_i;
    CHECK((i + n) == (n + i));
    for (difference_type_t<Iterator> x = 0; x <= n && x < 8; ++x)
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
