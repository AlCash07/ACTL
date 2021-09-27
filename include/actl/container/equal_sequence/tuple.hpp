#pragma once

#include <actl/category/tuple.hpp>
#include <actl/meta/type_traits.hpp>
#include <tuple>

namespace ac {

namespace detail {

template <class T, class U, size_t... Is>
constexpr bool equal_tuple(
    const T& lhs, const U& rhs, std::index_sequence<Is...>) noexcept
{
    static_assert(noexcept((... && (std::get<Is>(lhs) == std::get<Is>(rhs)))));
    return (... && (std::get<Is>(lhs) == std::get<Is>(rhs)));
}

} // namespace detail

template <class T, class U, enable_int_if<is_tuple_v<T> && is_tuple_v<U>> = 0>
constexpr auto equal_sequence(const T& lhs, const U& rhs) noexcept
{
    if constexpr (std::tuple_size_v<T> != std::tuple_size_v<U>)
        return std::false_type{};
    else
        return detail::equal_tuple(
            lhs, rhs, std::make_index_sequence<std::tuple_size_v<T>>{});
}

} // namespace ac
