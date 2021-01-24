// Rebind operates on classes that have one primary template parameter (which is
// the first one by default) and replaces all its occurrences with the given
// type. Inner template classes are modified only if their primary template
// parameter is the type that's being rebinded.
//
// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/template_type.hpp>
#include <functional>

namespace ac {

template <class T, class To>
struct rebind;

namespace detail {

template <class T, class To, bool = true>
struct rebind2 : rebind<T, To> {};

template <class T, class To>
struct rebind2<T, To, false> {
    using type = T;
};

template <class T, class To, class From>
struct rebind1
    : rebind2<T, To, std::is_same_v<typename template_type<T>::type, From>> {};

template <class To, class From>
struct rebind1<From, To, From> {
    using type = To;
};

template <class T, class To>
struct rebind0 {
    using type = T;
};

template <template <class...> class C, class... Ts, class To>
struct rebind0<C<Ts...>, To> {
    using type = C<typename rebind1<Ts, To, nth_type_t<0, Ts...>>::type...>;
};

} // namespace detail

template <class T, size_t N, class To>
struct rebind<T[N], To> {
    using type = To[N];
};

// Transparent operator function objects support.
// TODO: switch completely to ACTL operations.
template <class T, class To>
struct rebind<std::less<T>, To> {
    using type = std::less<>;
};

template <class T, class To>
struct rebind<std::equal_to<T>, To> {
    using type = std::equal_to<>;
};

template <class T, class To>
struct rebind : detail::rebind0<T, To> {};

template <class T, class To>
using rebind_t = typename rebind<T, To>::type;

} // namespace ac
