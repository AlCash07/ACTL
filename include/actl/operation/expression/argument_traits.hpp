// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/traits/associated_types.hpp>

namespace ac {

template<typename T>
struct raw {
    using type = T;
};

template<typename T>
using raw_t = typename raw<std::remove_cvref_t<T>>::type;

namespace detail {

template<bool B, typename T>
struct value_if {
    using type = range_value_t<raw_t<T>>;
};

template<typename T>
struct value_if<false, T> {
    using type = T;
};

template<bool B, typename T>
using value_type_if_t = typename value_if<B, T>::type;

} // namespace detail

template<typename T>
struct value_if_cheap {
    using type = T;
};

template<typename T>
struct value_if_cheap<T&> {
    static constexpr bool is_copy_cheap =
        std::is_empty_v<T> || std::is_arithmetic_v<T>;
    // sizeof(T) <= sizeof(void*) && std::is_trivially_copyable_v<T>;

    using type =
        std::conditional_t<is_copy_cheap, std::remove_const_t<T>, T const&>;
};

template<typename T>
using value_if_cheap_t = typename value_if_cheap<T>::type;

} // namespace ac
