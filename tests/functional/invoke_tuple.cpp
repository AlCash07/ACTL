// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/invoke_tuple.hpp>
#include <actl/meta/type_traits.hpp>
#include "test.hpp"

namespace {

/* invoke_first_matching */

struct int_cref_op
{
    constexpr int operator()(int const& x) const
    {
        if (x < 0)
            throw;
        return 0;
    }
};

struct other_op
{
    constexpr int operator()(int&&) const noexcept
    {
        return 1;
    }
    constexpr int operator()(std::byte const&) const noexcept
    {
        return 2;
    }
    constexpr int operator()(std::byte&&) const noexcept
    {
        return 3;
    }
};

constexpr std::tuple<int_cref_op, other_op> t2;
/* The first matching element is chosen even if the match isn't perfect */
static_assert(0 == ac::invoke_first_matching(t2, int{}));
/* Arguments are perfectly forwarded */
constexpr std::byte b{};
static_assert(2 == ac::invoke_first_matching(t2, b));
static_assert(3 == ac::invoke_first_matching(t2, std::byte{}));
/* noexcept is correctly propagated */
static_assert(!noexcept(ac::invoke_first_matching(t2, int{-1})));
static_assert(noexcept(ac::invoke_first_matching(t2, b)));

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
