/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <tuple>

namespace ac::io {

template <class T, class = void>
struct is_manipulator : std::false_type {};

template <class T>
struct is_manipulator<T, std::void_t<typename T::is_manipulator>> : std::true_type {};

template <class Format, class Manipulator>
inline void manipulate(Format&, const Manipulator&) {}

struct change_level {
    bool deeper;

    struct is_manipulator;
};

}  // namespace ac::io
