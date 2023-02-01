// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/tuple/invoke_all_matching.hpp>
#include <actl/meta/type_traits.hpp>
#include "test.hpp"

namespace {

template <class... Ts>
struct op
{
    bool invoked = false;

    template <class T>
        requires ac::is_one_of_v<T, Ts...>
    constexpr void operator()(T x) noexcept(std::is_same_v<T, int>)
    {
        invoked = true;
    }

    constexpr operator bool() const noexcept
    {
        return invoked;
    }
};

} // namespace

TEST_CASE("invoke_all_matching")
{
    std::tuple<op<int>, op<std::byte>, op<int, std::byte>> t3;
    SECTION("invoke with int")
    {
        ac::invoke_all_matching(t3, int{});
        CHECK(std::tuple{true, false, true} == t3);
    }
    SECTION("invoke with std::byte")
    {
        ac::invoke_all_matching(t3, std::byte{});
        CHECK(std::tuple{false, true, true} == t3);
    }
    /* noexcept is correctly propagated */
    static_assert(noexcept(ac::invoke_all_matching(t3, int{})));
    static_assert(!noexcept(ac::invoke_all_matching(t3, std::byte{})));
}
