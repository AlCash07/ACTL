#pragma once

#include <actl/container/equal_sequence/range.hpp>
#include <actl/container/equal_sequence/tuple.hpp>

namespace ac {

namespace detail {

template <class T, class U, size_t... Is>
constexpr bool equal_tuple_range(
    const T& lhs, const U& rhs, std::index_sequence<Is...>) noexcept
{
    static_assert(noexcept((... && (std::get<Is>(lhs) == rhs[Is]))));
    return (... && (std::get<Is>(lhs) == rhs[Is]));
}

} // namespace detail

template <
    class T,
    class U,
    enable_int_if<is_tuple_v<T> && detail::is_range_seq_v<U>> = 0>
constexpr bool equal_sequence(const T& lhs, const U& rhs) noexcept
{
    constexpr size_t n = std::tuple_size_v<T>;
    if (rhs.size() != n)
        return false;
    return detail::equal_tuple_range(lhs, rhs, std::make_index_sequence<n>{});
}

template <
    class T,
    class U,
    enable_int_if<detail::is_range_seq_v<T> && is_tuple_v<U>> = 0>
constexpr bool equal_sequence(const T& lhs, const U& rhs) noexcept
{
    return equal_sequence(rhs, lhs);
}

} // namespace ac
