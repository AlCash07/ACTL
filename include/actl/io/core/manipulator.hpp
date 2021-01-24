// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac::io {

template <class T, class = void>
struct is_manipulator : std::false_type {};

template <class T>
struct is_manipulator<T, std::void_t<typename T::is_manipulator>>
    : std::true_type {};

template <class Format, class Manipulator>
void manipulate(Format&, const Manipulator&) {}

template <bool Deeper>
struct change_level {
    struct is_manipulator;
};

template <class Format>
struct nested_scope_guard {
    explicit nested_scope_guard(Format& fmt) : fmt{fmt} {
        manipulate(fmt, change_level<true>{});
    }

    ~nested_scope_guard() {
        manipulate(fmt, change_level<false>{});
    }

    Format& fmt;
};

struct colon {};

} // namespace ac::io
