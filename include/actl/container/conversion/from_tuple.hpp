#pragma once

#include <actl/category/tuple.hpp>
#include <actl/container/conversion/convert_to.hpp>

namespace ac {

namespace detail {

template <
    class To,
    class From,
    class S = std::make_index_sequence<std::tuple_size_v<From>>>
struct from_tuple;

template <bool CanConstruct, class To, class From, size_t... Is>
struct from_tuple_impl
{
    static constexpr bool value = true;

    static constexpr To convert(From&& x) noexcept(noexcept(To{
        std::get<Is>(std::forward<From>(x))...}))
    {
        return To{std::get<Is>(std::forward<From>(x))...};
    }
};

template <class To, class From, size_t... Is>
struct from_tuple_impl<false, To, From, Is...>
{
    static constexpr bool can_convert()
    {
        if constexpr (is_tuple_v<To>)
            if constexpr (std::tuple_size_v<To> == std::tuple_size_v<From>)
                return (
                    ... && can_convert_to_v<
                               std::tuple_element_t<Is, To>,
                               std::tuple_element_t<Is, From>>);
        return false;
    }

    static constexpr bool value = can_convert();

    static constexpr To convert(From&& x) noexcept(noexcept(To{
        convert_to<std::tuple_element_t<Is, To>>(
            std::get<Is>(std::forward<From>(x)))...}))
    {
        return To{convert_to<std::tuple_element_t<Is, To>>(
            std::get<Is>(std::forward<From>(x)))...};
    }
};

template <class To, class From, size_t... Is>
struct from_tuple<To, From, std::index_sequence<Is...>>
    : from_tuple_impl<
          std::is_constructible_v<To, std::tuple_element_t<Is, From>...>,
          To,
          From,
          Is...>
{};

} // namespace detail

template <class To, class From>
struct conversion_sfinae<To, From, std::enable_if_t<is_tuple_v<From>>>
    : detail::from_tuple<To, From>
{};

} // namespace ac
