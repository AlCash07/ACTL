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
    using sa2 = ac::static_array<int, 3, 1>;
    constexpr sa2 array;
    static_assert(array == sa2{3, 1});
    static_assert(array == sa2{3_c, 1_c});
    CHECK_THROWS(sa2{3, 2});
    static_assert(!std::is_constructible_v<sa2, int>);
    static_assert(!std::is_constructible_v<sa2, int, int, int>);
    static_assert(!std::is_constructible_v<sa2, int, void*>);
    static_assert(!std::is_constructible_v<sa2, decltype(2_c), int>);
    /* CTAD */
#ifndef _MSC_VER // internal compiler error
    static_assert(ac::equal_same_type(array, ac::static_array{3_c, 1_c}));
#endif
}

} // namespace

TEST_CASE("static_array")
{
    using empty_sa = ac::static_array<size_t>;
    using sa2 = ac::static_array<int, 3, -1>;
    test_static_array_type_traits<empty_sa>();
    test_static_array_type_traits<sa2>();
    test_static_array_constructors();
    constexpr sa2 array;
    /* size */ {
        static_assert(0 == empty_sa{}.size());
        static_assert(2 == array.size());
    }
    /* element access */ {
        static_assert(3 == array[0]);
        static_assert(ac::equal_same_type(3_c, array[0_c]));
        static_assert(-1 == array[1]);
        static_assert(
            ac::equal_same_type(std::integral_constant<int, -1>{}, array[1_c]));
    }
    SECTION("structured binding")
    {
        static_assert(noexcept(array.get<1>()));
        auto [x0, x1] = array; // can't be constexpr yet
        CHECK(3 == x0);
        CHECK(-1 == x1);
    }
}
