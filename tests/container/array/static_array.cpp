// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/static_array.hpp>
#include <actl/integral_constant.hpp>
#include <actl_test/base/concept/regular.hpp>
#include <actl_test/base/equal_same_type.hpp>

using namespace ac::constant_literals;

namespace {

template <class StaticArray>
void test_static_array_type_traits()
{
    ac::test_trivially_semiregular_type_traits<StaticArray>();
    ac::test_nothrow_regular_type_traits<StaticArray>();
    static_assert(std::is_standard_layout_v<StaticArray>);
    static_assert(std::is_empty_v<StaticArray>);
}

void test_static_array_constructors()
{
    using sa31 = ac::static_array<int, 3, 1>;
    constexpr sa31 array;
    static_assert(array == sa31{3, 1});
    static_assert(array == sa31{3_c, 1_c});
    CHECK_THROWS(sa31{3, 2});
    static_assert(!std::is_constructible_v<sa31, int>);
    static_assert(!std::is_constructible_v<sa31, int, int, int>);
    static_assert(!std::is_constructible_v<sa31, int, void*>);
    static_assert(!std::is_constructible_v<sa31, decltype(2_c), int>);
    /* CTAD */
#ifndef _MSC_VER // internal compiler error
    static_assert(ac::equal_same_type(array, ac::static_array{3_c, 1_c}));
#endif
}

template <auto X>
inline constexpr auto const_v = std::integral_constant<decltype(X), X>{};

template <class T, T... Values, size_t... Is>
void test_static_array_interface_impl(std::index_sequence<Is...>)
{
    using Array = ac::static_array<T, Values...>;
    constexpr Array array;
    /* size */
    static_assert(noexcept(Array::size()));
    static_assert(sizeof...(Values) == Array::size());
    /* data */
    static_assert(noexcept(Array::data()));
    static_assert(((Values == Array::data()[Is]) && ...));
    /* element access */
    // TODO: fix this on MSVC
    // static_assert(ACTL_ASSERT_IS_NOEXCEPT() == noexcept(array[0]));
    static_assert(((Values == array[Is]) && ...));
    static_assert(((noexcept(array[const_v<Is>])) && ...));
    static_assert(
        (ac::equal_same_type(const_v<Values>, array[const_v<Is>]) && ...));
    static_assert(((noexcept(array.template get<Is>())) && ...));
    static_assert(
        (ac::equal_same_type(array[const_v<Is>], array.template get<Is>()) &&
         ...));
    /* equality */
    static_assert(noexcept(array == array));
    static_assert(array == array);
    static_assert(noexcept(array != array));
    static_assert(!(array != array));
}

template <class T, T... Values>
void test_static_array_interface()
{
    test_static_array_interface_impl<T, Values...>(
        std::make_index_sequence<sizeof...(Values)>{});
}

} // namespace

TEST_CASE("static_array")
{
    test_static_array_type_traits<ac::static_array<size_t>>();
    test_static_array_type_traits<ac::static_array<int, 3, -1>>();
    test_static_array_constructors();
    test_static_array_interface<unsigned int>();
    test_static_array_interface<size_t, 4>();
    test_static_array_interface<int, 5, 3, 2>();
    SECTION("structured binding")
    {
        auto [x0, x1] = ac::static_array<int, 3, -1>{}; // cant be constexpr yet
        CHECK(3 == x0);
        CHECK(-1 == x1);
    }
}
