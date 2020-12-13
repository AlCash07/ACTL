// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/util/type_traits.hpp>

namespace ac::math {

template <class T>
struct raw {
    using type = T;
};

template <class T>
using raw_t = typename raw<remove_cvref_t<T>>::type;

struct scalar_tag {};

template <class T, class = void>
struct category_impl {
    using type = scalar_tag;
};

template <class T>
struct category_impl<T, std::void_t<typename T::category>> {
    using type = typename T::category;
};

template <class T>
struct category : category_impl<T> {};

template <class T>
using category_t = typename category<raw_t<T>>::type;

namespace detail {

template <class Tag, class = void>
struct category_level : index_constant<0> {};

template <class T>
constexpr index category_level_v = category_level<T>::value;

template <class Tag>
struct category_level<Tag, std::void_t<typename Tag::base>>
    : index_constant<1 + category_level_v<typename Tag::base>> {};

template <bool B, class T>
struct value_if {
    using type = value_t<raw_t<T>>;
};

template <class T>
struct value_if<false, T> {
    using type = T;
};

template <bool B, class T>
using value_if_t = typename value_if<B, T>::type;

}  // namespace detail

template <class T, class U = remove_cvref_t<T>>
using value_if_small = std::conditional_t<std::is_empty_v<U> || std::is_arithmetic_v<U>, U, T>;

// pass is the same as std::forward except it converts reference into const reference
template <class T>
constexpr T&& pass(std::remove_reference_t<T>& x) {
    return static_cast<T&&>(x);
}

template <class T>
constexpr const T&& pass(std::remove_reference_t<T>&& x) {
    static_assert(!std::is_lvalue_reference_v<T>, "can not pass an rvalue as an lvalue");
    return static_cast<const T&&>(x);
}

template <class T>
constexpr T eval(const T& x) {
    return x;
}

template <class T, size_t N>
auto& eval(const T (&x)[N]) {
    return x;
}

}  // namespace ac::math