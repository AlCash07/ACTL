#pragma once

#include <actl/container/array/semi_static_array.hpp>

namespace ac {

template <class T, T... Values>
struct semi_static_array_selector
{
    static constexpr size_t size = sizeof...(Values);

    template <size_t SizeDynamic>
    struct impl
    {
        using type = ac::semi_static_array<T, Values...>;
    };

    template <>
    struct impl<0>
    {
        using type = ac::static_array<T, Values...>;
    };

    // When size is 0, we should prefer the previous specialization.
    // To do that, we disable this specialization by setting 1 instead of 0.
    template <>
    struct impl<size == 0 ? 1 : size>
    {
        using type = std::array<T, size>;
    };

    using type =
        typename impl<(0 + ... + size_t{Values == dynamic_extent<T>})>::type;
};

template <class T, T... Values>
using semi_static_array_t =
    typename semi_static_array_selector<T, Values...>::type;

} // namespace ac
