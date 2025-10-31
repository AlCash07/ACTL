// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ac {

template<typename Target, typename Source>
struct copy_reference {
    using type = Target;
};
template<typename Target, typename Source>
using copy_reference_t = typename copy_reference<Target, Source>::type;

template<typename Target, typename Source>
struct copy_const {
    using type = Target;
};
template<typename Target, typename Source>
using copy_const_t = typename copy_const<Target, Source>::type;

template<typename Target, typename Source>
struct copy_volatile {
    using type = Target;
};
template<typename Target, typename Source>
using copy_volatile_t = typename copy_volatile<Target, Source>::type;

template<typename Target, typename Source>
struct copy_cv {
    using type = copy_const_t<copy_volatile_t<Target, Source>, Source>;
};
template<typename Target, typename Source>
using copy_cv_t = typename copy_cv<Target, Source>::type;

template<typename Target, typename Source>
struct copy_cvref {
    using type = copy_reference_t<
        copy_cv_t<
            std::remove_reference_t<Target>,
            std::remove_reference_t<Source>>,
        Source>;
};
template<typename Target, typename Source>
using copy_cvref_t = typename copy_cvref<Target, Source>::type;

/* implementation */

template<typename Target, typename Source>
    requires(!std::is_reference_v<Target>)
struct copy_reference<Target, Source&> {
    using type = Target&;
};
template<typename Target, typename Source>
    requires(!std::is_reference_v<Target>)
struct copy_reference<Target, Source&&> {
    using type = Target&&;
};
template<typename Target, typename Source>
struct copy_reference<Target&, Source> : copy_reference<Target, Source> {};
template<typename Target, typename Source>
struct copy_reference<Target&&, Source> : copy_reference<Target, Source> {};

template<typename Target, typename Source>
    requires(!std::is_const_v<Target>)
struct copy_const<Target, Source const> {
    using type = Target const;
};
template<typename Target, typename Source>
struct copy_const<Target const, Source> : copy_const<Target, Source> {};

template<typename Target, typename Source>
    requires(!std::is_volatile_v<Target>)
struct copy_volatile<Target, Source volatile> {
    using type = Target volatile;
};
template<typename Target, typename Source>
struct copy_volatile<Target volatile, Source>
    : copy_volatile<Target, Source> {};

} // namespace ac
