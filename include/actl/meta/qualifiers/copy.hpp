// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ac {

template<class Dst, class Src>
struct copy_reference {
    using type = Dst;
};
template<class Dst, class Src>
using copy_reference_t = typename copy_reference<Dst, Src>::type;

template<class Dst, class Src>
struct copy_const {
    using type = Dst;
};
template<class Dst, class Src>
using copy_const_t = typename copy_const<Dst, Src>::type;

template<class Dst, class Src>
struct copy_volatile {
    using type = Dst;
};
template<class Dst, class Src>
using copy_volatile_t = typename copy_volatile<Dst, Src>::type;

template<class Dst, class Src>
struct copy_cv {
    using type = copy_const_t<copy_volatile_t<Dst, Src>, Src>;
};
template<class Dst, class Src>
using copy_cv_t = typename copy_cv<Dst, Src>::type;

template<class Dst, class Src>
struct copy_cvref {
    using type = copy_reference_t<
        copy_cv_t<std::remove_reference_t<Dst>, std::remove_reference_t<Src>>,
        Src>;
};
template<class Dst, class Src>
using copy_cvref_t = typename copy_cvref<Dst, Src>::type;

/* implementation */

template<class Dst, class Src>
    requires(!std::is_reference_v<Dst>)
struct copy_reference<Dst, Src&> {
    using type = Dst&;
};
template<class Dst, class Src>
    requires(!std::is_reference_v<Dst>)
struct copy_reference<Dst, Src&&> {
    using type = Dst&&;
};
template<class Dst, class Src>
struct copy_reference<Dst&, Src> : copy_reference<Dst, Src> {};
template<class Dst, class Src>
struct copy_reference<Dst&&, Src> : copy_reference<Dst, Src> {};

template<class Dst, class Src>
    requires(!std::is_const_v<Dst>)
struct copy_const<Dst, Src const> {
    using type = Dst const;
};
template<class Dst, class Src>
struct copy_const<Dst const, Src> : copy_const<Dst, Src> {};

template<class Dst, class Src>
    requires(!std::is_volatile_v<Dst>)
struct copy_volatile<Dst, Src volatile> {
    using type = Dst volatile;
};
template<class Dst, class Src>
struct copy_volatile<Dst volatile, Src> : copy_volatile<Dst, Src> {};

} // namespace ac
