#pragma once

#include <actl/meta/type_traits.hpp>

namespace ac {

template <class To, class... Args>
struct conversion_default : std::true_type
{
    static constexpr To convert(Args&&... xs) noexcept(
        noexcept(To(std::forward<Args>(xs)...)))
    {
        return To(std::forward<Args>(xs)...);
    }
};

template <class AlwaysVoid, class To, class... Args>
struct conversion_sfinae : std::false_type
{};

template <class To, class... Args>
struct conversion
    : std::conditional_t<
          std::is_constructible_v<To, Args...>,
          conversion_default<To, Args...>,
          conversion_sfinae<void, To, Args...>>
{};

template <class To, class... Args>
inline constexpr bool can_convert_to_v = conversion<To, Args...>::value;

template <
    class To,
    class... Args,
    enable_int_if<can_convert_to_v<To, Args...>> = 0>
constexpr To convert_to(Args&&... xs) noexcept(
    noexcept(conversion<To, Args...>::convert(std::forward<Args>(xs)...)))
{
    return conversion<To, Args...>::convert(std::forward<Args>(xs)...);
}

} // namespace ac
