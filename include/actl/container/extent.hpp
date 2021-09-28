#pragma once

#include <limits>
#include <type_traits>

namespace ac {

inline constexpr size_t dynamic_extent = std::numeric_limits<size_t>::max();

template <size_t N>
using size_constant = std::integral_constant<size_t, N>;

template <size_t StaticExtent>
using extent_holder_t = std::conditional_t<
    StaticExtent == dynamic_extent,
    size_t,
    size_constant<StaticExtent>>;

template <class T>
struct static_value
{
    static constexpr size_t value = dynamic_extent;
};

template <size_t N>
struct static_value<size_constant<N>>
{
    static constexpr size_t value = N;
};

template <class T>
inline constexpr size_t static_v = static_value<T>::value;

} // namespace ac
