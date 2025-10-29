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

template<typename T, typename To>
struct rebind;

namespace detail {

template<typename T, typename To, bool = true>
struct rebind2 : rebind<T, To> {};

template<typename T, typename To>
struct rebind2<T, To, false> {
    using type = T;
};

template<typename T, typename To, typename From>
struct rebind1
    : rebind2<T, To, std::is_same_v<typename template_type<T>::type, From>> {};

template<typename To, typename From>
struct rebind1<From, To, From> {
    using type = To;
};

template<typename T, typename To>
struct rebind0 {
    using type = T;
};

template<
    template<typename...> typename C,
    typename T0,
    typename... Ts,
    typename To>
struct rebind0<C<T0, Ts...>, To> {
    using type = C<To, typename rebind1<Ts, To, T0>::type...>;
};

} // namespace detail

template<typename T, size_t N, typename To>
struct rebind<T[N], To> {
    using type = To[N];
};

// Transparent operator function objects support.
// TODO: switch completely to ACTL operations.
template<typename T, typename To>
struct rebind<std::less<T>, To> {
    using type = std::less<>;
};

template<typename T, typename To>
struct rebind<std::equal_to<T>, To> {
    using type = std::equal_to<>;
};

template<typename T, typename To>
struct rebind : detail::rebind0<T, To> {};

template<typename T, typename To>
using rebind_t = typename rebind<T, To>::type;

} // namespace ac
