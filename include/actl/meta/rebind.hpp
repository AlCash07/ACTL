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

template<typename T, typename Target>
struct rebind;

namespace detail {

template<typename T, typename Target, bool = true>
struct rebind2 : rebind<T, Target> {};

template<typename T, typename Target>
struct rebind2<T, Target, false> {
    using type = T;
};

template<typename T, typename Target, typename Source>
struct rebind1
    : rebind2<
          T,
          Target,
          std::is_same_v<typename template_type<T>::type, Source>> {};

template<typename Target, typename Source>
struct rebind1<Source, Target, Source> {
    using type = Target;
};

template<typename T, typename Target>
struct rebind0 {
    using type = T;
};

template<
    template<typename...>
    typename C,
    typename T0,
    typename... Ts,
    typename Target>
struct rebind0<C<T0, Ts...>, Target> {
    using type = C<Target, typename rebind1<Ts, Target, T0>::type...>;
};

} // namespace detail

template<typename T, size_t N, typename Target>
struct rebind<T[N], Target> {
    using type = Target[N];
};

// Transparent operator function objects support.
// TODO: switch completely to ACTL operations.
template<typename T, typename Target>
struct rebind<std::less<T>, Target> {
    using type = std::less<>;
};

template<typename T, typename Target>
struct rebind<std::equal_to<T>, Target> {
    using type = std::equal_to<>;
};

template<typename T, typename Target>
struct rebind : detail::rebind0<T, Target> {};

template<typename T, typename Target>
using rebind_t = typename rebind<T, Target>::type;

} // namespace ac
