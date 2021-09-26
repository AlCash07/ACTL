#pragma once

#include <actl/assert.hpp>
#include <actl/container/conversion/convert.hpp>

namespace ac {

template <class To, To X, class From>
struct conversion<std::integral_constant<To, X>, From>
{
    static constexpr bool value = std::is_constructible_v<To, From>;

    static constexpr std::integral_constant<To, X> convert(From&& x) noexcept(
        ACTL_ASSERT_IS_NOEXCEPT())
    {
        ACTL_ASSERT(x == X);
        return {};
    }
};

template <class To, To X, class From, From Y>
struct conversion<
    std::integral_constant<To, X>,
    std::integral_constant<From, Y>>
{
    static constexpr bool value = X == Y;

    static constexpr std::integral_constant<To, X> convert(From) noexcept
    {
        return {};
    }
};

} // namespace ac
