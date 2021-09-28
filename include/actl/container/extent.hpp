#pragma once

#include <limits>
#include <type_traits>

namespace ac {

template <class T = size_t>
inline constexpr T dynamic_extent = static_cast<T>(-1);

template <size_t N>
using size_constant = std::integral_constant<size_t, N>;

template <auto StaticExtent, class T = decltype(StaticExtent)>
using extent_holder_t = std::conditional_t<
    StaticExtent == dynamic_extent<T>,
    T,
    std::integral_constant<T, StaticExtent>>;

template <class T>
struct static_value
{
    static constexpr T value = dynamic_extent<T>;
};

template <class T, T N>
struct static_value<std::integral_constant<T, N>>
{
    static constexpr T value = N;
};

template <class T>
inline constexpr auto static_v = static_value<T>::value;

} // namespace ac
