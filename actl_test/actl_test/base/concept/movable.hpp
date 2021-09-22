// https://en.cppreference.com/w/cpp/concepts/movable

#pragma once

#include <type_traits>

namespace ac {

template <class T>
void test_movable_type_traits()
{
    static_assert(std::is_object_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_swappable_v<T>);
    // This is not included in the concept requirement for some reason.
    static_assert(std::is_destructible_v<T>);
}

template <class T>
void test_trivially_movable_type_traits()
{
    test_movable_type_traits<T>();
    static_assert(std::is_trivially_move_constructible_v<T>);
    static_assert(std::is_trivially_move_assignable_v<T>);
    static_assert(std::is_trivially_destructible_v<T>);
}

template <class T>
void test_nothrow_movable_type_traits()
{
    static_assert(std::is_object_v<T>);
    static_assert(std::is_nothrow_move_constructible_v<T>);
    static_assert(std::is_nothrow_move_assignable_v<T>);
    static_assert(std::is_nothrow_swappable_v<T>);
    static_assert(std::is_nothrow_destructible_v<T>);
}

} // namespace ac
