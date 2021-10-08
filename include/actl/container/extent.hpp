#pragma once

#include <actl/meta/type_traits.hpp>
#include <actl/utility/use_default.hpp>
#include <limits>

namespace ac {

template <class T = size_t>
inline constexpr T dynamic_extent = static_cast<T>(-1);

template <class T, T StaticExtent>
using extent_holder_t = std::conditional_t<
    StaticExtent == dynamic_extent<T>,
    T,
    std::integral_constant<T, StaticExtent>>;

template <class T, class Dst>
struct static_extent
{
    static constexpr auto value = dynamic_extent<deduce_t<Dst, T>>;
};

template <class T, T N, class Dst>
struct static_extent<std::integral_constant<T, N>, Dst>
{
    static constexpr auto value = deduce_t<Dst, T>{N};
};

template <class T, class Dst = use_default>
inline constexpr auto static_extent_v = static_extent<T, Dst>::value;

} // namespace ac
