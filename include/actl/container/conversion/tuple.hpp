#pragma once

#include <actl/category/tuple.hpp>
#include <actl/container/conversion/convert.hpp>
#include <tuple>

namespace ac {

template <
    class To,
    class From,
    class S = std::make_index_sequence<std::tuple_size_v<To>>>
struct tuple_conversion;

template <class To, class From, size_t... Is>
struct tuple_conversion<To, From, std::index_sequence<Is...>>
{
    static constexpr bool value =
        (... && can_convert_v<
                    std::tuple_element_t<Is, To>,
                    std::tuple_element_t<Is, From>>);

    static constexpr To convert(From&& x) noexcept(noexcept(To{
        ac::convert<std::tuple_element_t<Is, To>>(
            std::get<Is>(std::forward<From>(x)))...}))
    {
        return To{ac::convert<std::tuple_element_t<Is, To>>(
            std::get<Is>(std::forward<From>(x)))...};
    }
};

template <class To, class From>
struct conversion<
    To,
    From,
    std::enable_if_t<is_tuple_v<To> && is_tuple_v<From>>>
    : std::conditional_t<
          std::tuple_size_v<To> == std::tuple_size_v<From>,
          tuple_conversion<To, From>,
          std::false_type>
{};

} // namespace ac
