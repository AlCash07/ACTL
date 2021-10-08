#pragma once

#include <actl/category/tuple.hpp>
#include <actl/container/conversion/convert_to.hpp>

namespace ac {

namespace detail {

template <class S, class To, class... Args>
struct to_tuple_impl;

template <size_t... Is, class To, class... Args>
struct to_tuple_impl<std::index_sequence<Is...>, To, Args...>
{
    static constexpr bool value =
        (... && can_convert_to_v<std::tuple_element_t<Is, To>, Args>);

    static constexpr To convert(Args&&... xs) noexcept(noexcept(To{
        convert_to<std::tuple_element_t<Is, To>>(std::forward<Args>(xs))...}))
    {
        return To{convert_to<std::tuple_element_t<Is, To>>(
            std::forward<Args>(xs))...};
    }
};

template <class To, class... Args>
using to_tuple =
    to_tuple_impl<std::make_index_sequence<std::tuple_size_v<To>>, To, Args...>;

template <class To, class... Args>
static constexpr bool can_initialize_tuple()
{
    // Avoid conflicts with from_tuple specialization.
    if constexpr (
        sizeof...(Args) == 1 && (... && is_tuple_v<remove_cvref_t<Args>>))
        return false;
    // Arrays and tuples may allow to initialize not all their elements but only
    // some of the first ones. Our conversions intentionnally forbid this.
    if constexpr (is_tuple_v<To>)
        if constexpr (std::tuple_size_v<To> == sizeof...(Args))
            return to_tuple<To, Args...>::value;
    return false;
}

} // namespace detail

template <class To, class... Args>
struct conversion_sfinae<
    std::enable_if_t<detail::can_initialize_tuple<To, Args...>()>,
    To,
    Args...> : detail::to_tuple<To, Args...>
{};

} // namespace ac