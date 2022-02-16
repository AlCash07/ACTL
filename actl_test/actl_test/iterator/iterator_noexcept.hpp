// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/dependent.hpp>

namespace ac {

template <class Iterator, class Category>
void test_iterator_noexcept(Iterator iter, Category tag)
{
    test_iterator_noexcept_traversal(iter, tag);
    test_iterator_noexcept_construction(iter, tag);
}

template <class Iterator>
void test_iterator_noexcept_traversal(Iterator iter, std::input_iterator_tag)
{
    static_assert(noexcept(*iter));
    static_assert(noexcept(++iter));
}

template <class Iterator>
void test_iterator_noexcept_construction(Iterator iter, std::input_iterator_tag)
{
    static_assert(std::is_nothrow_move_constructible_v<Iterator>);
    static_assert(std::is_nothrow_move_assignable_v<Iterator>);
}

template <class Iterator>
void test_iterator_noexcept_construction(
    Iterator iter, std::forward_iterator_tag)
{
    test_iterator_noexcept_construction(iter, std::input_iterator_tag{});
    static_assert(std::is_nothrow_copy_constructible_v<Iterator>);
    static_assert(std::is_nothrow_copy_assignable_v<Iterator>);
    static_assert(noexcept(iter++));
}

template <class Iterator>
void test_iterator_noexcept_traversal(
    Iterator iter, std::bidirectional_iterator_tag)
{
    test_iterator_noexcept_traversal(iter, std::forward_iterator_tag{});
    static_assert(noexcept(--iter));
}

template <class Iterator>
void test_iterator_noexcept_construction(
    Iterator iter, std::bidirectional_iterator_tag)
{
    test_iterator_noexcept_construction(iter, std::forward_iterator_tag{});
    static_assert(noexcept(iter--));
}

template <class Iterator>
void test_iterator_noexcept_traversal(
    Iterator iter, std::random_access_iterator_tag)
{
    test_iterator_noexcept_traversal(iter, std::bidirectional_iterator_tag{});
    test_relational_operators_noexcept(iter);
    static_assert(noexcept(iter - iter));
    difference_type_t<Iterator> n;
    static_assert(noexcept(iter += n));
    static_assert(noexcept(iter -= n));
    static_assert(noexcept(iter[n]));
}

template <class Iterator>
void test_iterator_noexcept_construction(
    Iterator iter, std::random_access_iterator_tag)
{
    test_iterator_noexcept_construction(
        iter, std::bidirectional_iterator_tag{});
    difference_type_t<Iterator> n;
    static_assert(noexcept(iter + n));
    static_assert(noexcept(n + iter));
    static_assert(noexcept(iter - n));
}

} // namespace ac
