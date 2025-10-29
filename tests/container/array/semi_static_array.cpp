// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/semi_static_array.hpp>
#include <actl/meta/concepts/object/regular.hpp>
#include <actl/meta/constant_literals.hpp>
#include <actl_test/base/equal_same_type.hpp>
#include <actl_test/base/regular.hpp>

using namespace ac::constant_literals;

namespace {

template<typename SemiStaticArray>
void test_semi_static_array_type_traits() {
    static_assert(ac::TriviallySemiregular<SemiStaticArray>);
    static_assert(ac::NothrowRegular<SemiStaticArray>);
    static_assert(std::is_standard_layout_v<SemiStaticArray>);
    static_assert(
        sizeof(SemiStaticArray) ==
        SemiStaticArray::size_dynamic() *
            sizeof(typename SemiStaticArray::value_type)
    );
}

void test_static_array_constructors() {
    using ssa3XX2 = ac::semi_static_array<int, 3, -1, -1, 2>;
    /* default constructor fills all the dynamic extents with 0 */
    static_assert(ssa3XX2{0, 0} == ssa3XX2{});
    constexpr ssa3XX2 array{5, 4};
    /* from dynamic values */
    static_assert(array == ssa3XX2{5_c, 4u});
    static_assert(!std::is_constructible_v<ssa3XX2, int>);
    static_assert(!std::is_constructible_v<ssa3XX2, int, int, int>);
    static_assert(!std::is_constructible_v<ssa3XX2, int, void*>);
    /* from all values */
    static_assert(array == ssa3XX2{3_c, 5, 4, 2_c});
    static_assert(!std::is_constructible_v<ssa3XX2, int, int, int, int, int>);
    static_assert(!std::is_constructible_v<
                  ssa3XX2,
                  decltype(4_c),
                  int,
                  int,
                  int>);
    static_assert(!std::is_constructible_v<ssa3XX2, int, void*, int, int>);
    /* CTAD */
    static_assert(
        ac::equal_same_type(array, ac::semi_static_array{3_c, 5, 4, 2_c})
    );
}

template<typename Array, typename D>
constexpr Array fill_dynamic_values(Array src, D dynamic) noexcept {
    size_t d = 0;
    for (size_t i = 0; i < src.size(); ++i)
        if (src[i] == ac::dynamic_extent<typename Array::value_type>)
            src[i] = dynamic[d++];
    return src;
}

template<typename T, T... StaticValues, T... DynamicValues, size_t... Is>
void test_semi_static_array_interface_impl(std::integer_sequence<T, DynamicValues...>, std::index_sequence<Is...>) {
    using ac::constant;
    using Array = ac::semi_static_array<T, StaticValues...>;
    constexpr Array array{DynamicValues...};
    // Can't write std::array{StaticValues...} because of a brand new MSVC bug.
    constexpr auto values = fill_dynamic_values(
        std::array<T, sizeof...(StaticValues)>{StaticValues...},
        std::array{DynamicValues...}
    );
    /* size */
    static_assert(noexcept(Array::size()));
    static_assert(sizeof...(StaticValues) == Array::size());
    static_assert(noexcept(Array::size_dynamic()));
    static_assert(sizeof...(DynamicValues) == Array::size_dynamic());
    /* element access */
    static_assert(((StaticValues == array.static_values[Is]) && ...));
    // TODO: fix this on MSVC
    // static_assert(AC_ASSERT_IS_NOEXCEPT() == noexcept(array[0]));
    static_assert(((values[Is] == array[Is]) && ...));
    static_assert(((noexcept(array[constant<Is>{}])) && ...));
    static_assert(
        (std::is_same_v<
             ac::extent_holder_t<T, StaticValues>,
             decltype(array[constant<Is>{}])> &&
         ...)
    );
    static_assert(((values[Is] == array[constant<Is>{}]) && ...));
    /* tuple interface */
    using std::get;
    static_assert(((noexcept(get<Is>(array))) && ...));
    static_assert(
        (ac::equal_same_type(array[constant<Is>{}], get<Is>(array)) && ...)
    );
}

template<typename T, T... StaticValues, typename DynamicValues>
void test_semi_static_array_interface(DynamicValues) {
    test_semi_static_array_interface_impl<T, StaticValues...>(
        DynamicValues{}, std::make_index_sequence<sizeof...(StaticValues)>{}
    );
}

} // namespace

TEST_CASE("semi_static_array") {
    static constexpr size_t dyn = ac::dynamic_extent<>;
    using ssa4X = ac::semi_static_array<int, 4, -1>;
    using ssaX3X2 = ac::semi_static_array<size_t, dyn, 3, dyn, 2>;
    test_semi_static_array_type_traits<ssa4X>();
    test_semi_static_array_type_traits<ssaX3X2>();
    ac::test_regular(ssaX3X2{5, 4}, ssaX3X2{4, 4});
    test_static_array_constructors();
    test_semi_static_array_interface<size_t, 4, dyn>(std::index_sequence<2>{});
    test_semi_static_array_interface<size_t, dyn, 2>(std::index_sequence<4>{});
    test_semi_static_array_interface<int, -1, 3, -1>(
        std::integer_sequence<int, 5, 2>{}
    );
    SECTION("dynamic element modification") {
        ssa4X mutable_array{5};
        CHECK(5 == mutable_array[1]);
        mutable_array[1_c] = 3;
        CHECK(3 == mutable_array[1]);
        mutable_array.dynamic_values[0] = 2;
        CHECK(2 == mutable_array[1]);
    }
    SECTION("structured binding") {
        auto [x0, x1] = ssa4X{2}; // can't be constexpr yet
        CHECK(ac::equal_same_type(4_c, x0));
        CHECK(ac::equal_same_type(2, x1));
    }
    /* equality */ {
        static_assert(noexcept(ssaX3X2{} == ssaX3X2{}));
        static_assert(noexcept(ssaX3X2{} != ssaX3X2{}));
        static_assert(ssaX3X2{5, 0} == ssaX3X2{5, 0});
        static_assert(!(ssaX3X2{5, 0} == ssaX3X2{}));
        static_assert(!(ssaX3X2{5, 0} == ssaX3X2{5, 1}));
    }
    /* conversion */ {
        static_assert(ac::can_convert_to_v<ssaX3X2, int, uint32_t>);
        static_assert(
            ssaX3X2{4, 5} == ac::convert_to<ssaX3X2>(int{4}, uint32_t{5})
        );
    }
}
