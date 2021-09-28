#pragma once

#include <actl/meta/type_traits.hpp>

namespace ac {

template <class To, class From, class = void>
struct conversion
{
    static constexpr bool value = std::is_constructible_v<To, From>;

    static constexpr To convert(From&& x) noexcept(noexcept(static_cast<To>(x)))
    {
        return static_cast<To>(x);
    }
};

template <class To, class From>
inline constexpr bool can_convert_to_v = conversion<To, From>::value;

template <class To, class From, enable_int_if<can_convert_to_v<To, From>> = 0>
constexpr To convert_to(From&& x) noexcept(
    noexcept(conversion<To, From>::convert(std::forward<From>(x))))
{
    return conversion<To, From>::convert(std::forward<From>(x));
}

} // namespace ac
