// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/semi_static_array.hpp>
#include <actl_test/base/concept/regular.hpp>
#include <actl_test/base/equal_same_type.hpp>

using namespace ac::constant_literals;

namespace {

template <class SemiStaticArray>
void test_semi_static_array_type_traits()
{
    ac::test_trivially_semiregular_type_traits<SemiStaticArray>();
    ac::test_nothrow_regular_type_traits<SemiStaticArray>();
    static_assert(std::is_standard_layout_v<SemiStaticArray>);
    static_assert(
        sizeof(SemiStaticArray) ==
        SemiStaticArray::size_dynamic() *
            sizeof(typename SemiStaticArray::value_type));
}

void test_static_array_constructors()
{
    using ssa4 = ac::semi_static_array<unsigned int, 3, -1u, -1u, 2>;
    constexpr ssa4 array{5, 4};
    /* from dynamic values */
    static_assert(array == ssa4{5_c, 4u});
    static_assert(!std::is_constructible_v<ssa4, int>);
    static_assert(!std::is_constructible_v<ssa4, int, int, int>);
    static_assert(!std::is_constructible_v<ssa4, int, void*>);
    /* from all values */
    static_assert(array == ssa4{3, 5u, 4, 2u});
    static_assert(array == ssa4{3_c, 5_c, 4, 2});
    CHECK_THROWS(ssa4{2, 2, 2, 2});
    static_assert(!std::is_constructible_v<ssa4, int, int, int, int, int>);
    static_assert(!std::is_constructible_v<ssa4, decltype(4_c), int, int, int>);
    static_assert(!std::is_constructible_v<ssa4, int, void*, int, int>);
    /* CTAD */
    static_assert(
        ac::equal_same_type(array, ac::semi_static_array{3_uc, 5u, 4u, 2_uc}));
}

} // namespace

TEST_CASE("semi_static_array")
{
    using ssa2 = ac::semi_static_array<size_t, 3, ac::dynamic_extent<>>;
    using ssa4 = ac::semi_static_array<int, -1, 3, -1, 2>;
    test_semi_static_array_type_traits<ssa2>();
    test_semi_static_array_type_traits<ssa4>();
    ac::test_regular(ssa4{5, 4}, ssa4{4, 4});
    test_static_array_constructors();
    SECTION("element access")
    {
        constexpr ssa4 array{5, 4};
        static_assert(4 == array.size());
        static_assert(ac::equal_same_type(3_c, array[1_c]));
        static_assert(ac::equal_same_type(2_c, array[3_c]));
        for (size_t i = 0; i < array.size(); ++i)
        {
            CHECK(std::array{5, 3, 4, 2}[i] == array[i]);
        }
    }
    SECTION("dynamic element modification")
    {
        ssa2 mutable_a{1};
        CHECK(1 == mutable_a[1]);
        mutable_a[1_c] = 5;
        CHECK(5 == mutable_a[1]);
    }
    SECTION("structured binding")
    {
        static_assert(noexcept(ssa2{}.get<1>()));
        auto [x0, x1] = ssa2{1}; // can't be constexpr yet
        CHECK(3 == x0);
        CHECK(1 == x1);
    }
}
