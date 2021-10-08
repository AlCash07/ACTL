#pragma once

#include <actl/category/tuple.hpp>
#include <actl/meta/can_list_initialize.hpp>

namespace ac {

template <class To, class... Args>
struct conversion_default : std::true_type
{
    static constexpr To convert(Args&&... xs) //
        noexcept(noexcept(To{std::forward<Args>(xs)...}))
    {
        return To{std::forward<Args>(xs)...};
    }
};

template <class AlwaysVoid, class To, class... Args>
struct conversion_sfinae : std::false_type
{};

template <class To, class From>
struct conversion_sfinae<
    std::enable_if_t<
        std::is_arithmetic_v<To> && std::is_convertible_v<From, To>>,
    To,
    From> : std::true_type
{
    static constexpr To convert(From&& x) //
        noexcept(noexcept(static_cast<To>(std::forward<From>(x))))
    {
        return static_cast<To>(std::forward<From>(x));
    }
};

// List initialization is intentionally used here instead of construction
// to avoid accidental calls to constructors such as std::vector(size_t n).
// To convert to a tuple (including an array), include to_tuple.hpp.
template <class To, class... Args>
struct conversion
    : std::conditional_t<
          !is_tuple_v<To> && can_list_initialize_v<To, Args...>,
          conversion_default<To, Args...>,
          conversion_sfinae<void, To, Args...>>
{};

template <class To, class... Args>
inline constexpr bool can_convert_to_v = conversion<To, Args...>::value;

template <class To, class... Args>
constexpr To convert_to(Args&&... xs) noexcept(
    noexcept(conversion<To, Args...>::convert(std::forward<Args>(xs)...)))
{
    return conversion<To, Args...>::convert(std::forward<Args>(xs)...);
}

} // namespace ac
