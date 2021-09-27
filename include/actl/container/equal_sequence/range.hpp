#pragma once

#include <actl/category/range.hpp>
#include <actl/category/tuple.hpp>
#include <actl/meta/type_traits.hpp>

namespace ac {

namespace detail {

template <class T>
inline constexpr bool is_range_seq_v =
    is_random_access_range_v<T> && !is_tuple_v<T>;

}

template <
    class T,
    class U,
    enable_int_if<detail::is_range_seq_v<T> && detail::is_range_seq_v<U>> = 0>
constexpr bool equal_sequence(const T& lhs, const U& rhs) noexcept
{
    static_assert(noexcept(lhs.size(), rhs.size(), lhs[0], rhs[0]));
    auto n = lhs.size();
    if (rhs.size() != n)
        return false;
    for (decltype(n) i = 0; i != n; ++i)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}

} // namespace ac
