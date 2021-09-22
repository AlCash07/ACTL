// https://en.cppreference.com/w/cpp/concepts/copyable

#pragma once

#include <actl_test/base/concept/movable.hpp>

namespace ac {

template <class T>
void test_copyable_type_traits()
{
    test_movable_type_traits<T>();
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
}

template <class T>
void test_trivially_copyable_type_traits()
{
    test_trivially_movable_type_traits<T>();
    static_assert(std::is_trivially_copy_constructible_v<T>);
    static_assert(std::is_trivially_copy_assignable_v<T>);
}

template <class T>
void test_nothrow_copyable_type_traits()
{
    test_nothrow_movable_type_traits<T>();
    static_assert(std::is_nothrow_copy_constructible_v<T>);
    static_assert(std::is_nothrow_copy_assignable_v<T>);
}

} // namespace ac
