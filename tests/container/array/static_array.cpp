// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/static_array.hpp>
#include <actl/meta/constant_literals.hpp>
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
    static_assert(array == sa31{3_c, 1_c});
    static_assert(!std::is_constructible_v<sa31, decltype(2_c), decltype(1_c)>);
    static_assert(!std::is_constructible_v<sa31, decltype(3_c), decltype(2_c)>);
    static_assert(!std::is_constructible_v<sa31, int, int>);
    static_assert(!std::is_constructible_v<sa31, int>);
    /* CTAD */
#ifndef _MSC_VER // internal compiler error
    static_assert(ac::equal_same_type(array, ac::static_array{3_c, 1_c}));
#endif
}

template <class T, T... Values, size_t... Is>
void test_static_array_interface_impl(std::index_sequence<Is...>)
{
    using ac::constant;
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
    // static_assert(AC_ASSERT_IS_NOEXCEPT() == noexcept(array[0]));
    static_assert(((Values == array[Is]) && ...));
    static_assert(((noexcept(array[constant<Is>{}])) && ...));
    static_assert((
        ac::equal_same_type(constant<Values>{}, array[constant<Is>{}]) && ...));
    /* tuple interface */
    using std::get;
    static_assert(((noexcept(get<Is>(array))) && ...));
    static_assert(
        (ac::equal_same_type(array[constant<Is>{}], get<Is>(array)) && ...));
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
