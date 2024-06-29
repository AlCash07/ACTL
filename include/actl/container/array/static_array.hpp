// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant.hpp>
#include <actl/range/interface/contiguous_range_interface.hpp>
#include <actl/std/array.hpp>

namespace ac {

/// @class static_array is an array with all the elements known at compile-time.
template<class T, T... Values>
class static_array
    : public contiguous_range_interface<static_array<T, Values...>> {
    using base_t = contiguous_range_interface<static_array<T, Values...>>;

    template<class... Ts>
    static constexpr bool can_construct_from() {
        if constexpr (sizeof...(Ts) == sizeof...(Values))
            return (... && std::is_same_v<constant<Values>, Ts>);
        else
            return false;
    }

    static constexpr std::array<T, sizeof...(Values)> array = {Values...};

public:
    static constexpr auto size() noexcept {
        return constant<array.size()>{};
    }

    static constexpr T const* data() noexcept {
        return array.data();
    }

    constexpr static_array() noexcept = default;

    // TODO: when std::is_trivial implementation is fixes in clang,
    // this can be simplified to static_array(t_constant<Values>...)
    template<class T0, class... Ts>
        requires(can_construct_from<T0, Ts...>())
    constexpr static_array(T0, Ts...) noexcept {}

    using base_t::operator[];

    /// Indexing by a compile-time constant results in a compile-time constant.
    template<auto I>
    constexpr auto operator[](constant<I>) const noexcept {
        static_assert(0 <= I && I < size());
        return constant<array[I]>{};
    }

    // Structured binding support.
    template<size_t I>
    friend constexpr auto get(static_array src) noexcept {
        return src[constant<I>{}];
    }

    friend constexpr void swap(static_array&, static_array&) noexcept {}

    friend constexpr std::true_type operator==(
        static_array, static_array
    ) noexcept {
        return {};
    }

    friend constexpr std::false_type operator!=(
        static_array, static_array
    ) noexcept {
        return {};
    }
};

template<class T, T... Values>
static_array(constant<Values>...) -> static_array<T, Values...>;

template<class T, T... Values>
struct range_properties<static_array<T, Values...>>
    : default_range_properties {};

} // namespace ac

namespace std {

template<class T, T... Values>
struct tuple_size<ac::static_array<T, Values...>> {
    static constexpr size_t value = sizeof...(Values);
};

template<size_t I, class T, T... Values>
struct tuple_element<I, ac::static_array<T, Values...>> {
    using type = decltype(get<I>(ac::static_array<T, Values...>{}));
};

} // namespace std
