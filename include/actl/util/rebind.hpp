/***************************************************************************************************
 * Rebind operates on classes that have one primary template parameter (which is the first one by
 * default) and replaces all its occurrences with the given type. Inner template classes are
 * modified only if their primary template parameter is the type that's being rebinded.
 * If available, nested rebind<T> or rebind<T>::other template is used to be consistent with
 * pointer_traits and allocator_traits.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/nth_type.hpp>
#include <type_traits>

namespace ac {

template <class T>
struct template_type {
    using type = T;
};

template <class T, size_t N>
struct template_type<T[N]> {
    using type = T;
};

template <template <class...> class C, class... Ts>
struct template_type<C<Ts...>> {
    using type = nth_type_t<0, Ts...>;
};

template <class T, class To>
struct rebind;

namespace detail {

template <class T, class To, bool = true>
struct rebind4 : rebind<T, To> {};

template <class T, class To>
struct rebind4<T, To, false> {
    using type = T;
};

template <class T, class To, class From>
struct rebind3 : rebind4<T, To, std::is_same_v<typename template_type<T>::type, From>> {};

template <class To, class From>
struct rebind3<From, To, From> {
    using type = To;
};

template <class T, class To>
struct rebind2 {
    using type = T;
};

template <template <class...> class C, class... Ts, class To>
struct rebind2<C<Ts...>, To> {
    using type = C<typename rebind3<Ts, To, nth_type_t<0, Ts...>>::type...>;
};

template <class R, class To, class = void>
struct rebind1 {
    using type = R;
};

// allocator support
template <class R, class To>
struct rebind1<R, To, std::void_t<typename R::other>> {
    using type = typename R::other;
};

template <class T, class To, class = void>
struct rebind0 : rebind2<T, To> {};

template <class T, class To>
struct rebind0<T, To, std::void_t<typename T::template rebind<To>>>
    : rebind1<typename T::template rebind<To>, To> {};

}  // namespace detail

template <class T, size_t N, class To>
struct rebind<T[N], To> {
    using type = To[N];
};

template <class T, class To>
struct rebind : detail::rebind0<T, To> {};

template <class T, class To>
using rebind_t = typename rebind<T, To>::type;

}  // namespace ac
