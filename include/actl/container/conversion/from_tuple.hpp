#pragma once

#include <actl/category/tuple.hpp>
#include <actl/container/conversion/convert_to.hpp>

namespace ac {

namespace detail {

template <size_t I, class Tuple>
constexpr decltype(auto) adl_get(const Tuple& x) noexcept
{
    using std::get;
    static_assert(noexcept(get<I>(x)));
    return get<I>(x);
}

template <
    class To,
    class From,
    class S = std::make_index_sequence<std::tuple_size_v<remove_cvref_t<From>>>>
struct from_tuple;

template <bool CanConstruct, class To, class From, size_t... Is>
struct from_tuple_impl
{
    static constexpr bool value = true;

    static constexpr To convert(From&& x) noexcept(noexcept(To{
        adl_get<Is>(std::forward<From>(x))...}))
    {
        return To{adl_get<Is>(std::forward<From>(x))...};
    }
};

template <class To, class From, size_t... Is>
struct from_tuple_impl<false, To, From, Is...>
{
    static constexpr bool can_convert()
    {
        if constexpr (is_tuple_v<To>)
            if constexpr (
                std::tuple_size_v<To> ==
                std::tuple_size_v<remove_cvref_t<From>>)
                return (
                    ... && can_convert_to_v<
                               std::tuple_element_t<Is, To>,
                               decltype(adl_get<Is>(std::declval<From>()))>);
        return false;
    }

    static constexpr bool value = can_convert();

    static constexpr To convert(From&& x) noexcept(noexcept(To{
        convert_to<std::tuple_element_t<Is, To>>(
            adl_get<Is>(std::forward<From>(x)))...}))
    {
        return To{convert_to<std::tuple_element_t<Is, To>>(
            adl_get<Is>(std::forward<From>(x)))...};
    }
};

template <class To, class From, size_t... Is>
struct from_tuple<To, From, std::index_sequence<Is...>>
    : from_tuple_impl<
          std::is_constructible_v<
              To,
              decltype(adl_get<Is>(std::declval<From>()))...>,
          To,
          From,
          Is...>
{};

} // namespace detail

template <class To, class From>
struct conversion_sfinae<
    std::enable_if_t<is_tuple_v<remove_cvref_t<From>>>,
    To,
    From> : detail::from_tuple<To, From>
{};

} // namespace ac
