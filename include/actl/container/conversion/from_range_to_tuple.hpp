#pragma once

#include <actl/assert.hpp>
#include <actl/container/conversion/to_tuple.hpp>
#include <actl/range/is_dynamic_range.hpp>

namespace ac {

namespace detail {

template <class S, class To, class From>
struct range_to_tuple_impl;

template <size_t... Is, class To, class From>
struct range_to_tuple_impl<std::index_sequence<Is...>, To, From>
{
    using from_ref = reference_t<std::remove_reference_t<From>>;

    static constexpr bool value =
        (... && can_convert_to_v<std::tuple_element_t<Is, To>, from_ref>);

    static constexpr To convert(From&& x) noexcept(
        ACTL_ASSERT_IS_NOEXCEPT() && noexcept(To{
            convert_to<std::tuple_element_t<Is, To>>(x[Is])...}))
    {
        ACTL_ASSERT(std::tuple_size_v<To> == x.size());
        return To{convert_to<std::tuple_element_t<Is, To>>(x[Is])...};
    }
};

template <class To, class From>
using range_to_tuple = range_to_tuple_impl<
    std::make_index_sequence<std::tuple_size_v<To>>,
    To,
    From>;

template <class To, class From>
static constexpr bool range_to_tuple_test()
{
    if constexpr (is_tuple_v<To> && is_dynamic_range_v<From>)
        return range_to_tuple<To, From>::value;
    return false;
}

} // namespace detail

template <class To, class From>
struct conversion_sfinae<
    std::enable_if_t<detail::range_to_tuple_test<To, From>()>,
    To,
    From> : detail::range_to_tuple<To, From>
{};

} // namespace ac
