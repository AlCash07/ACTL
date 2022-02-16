// https://en.cppreference.com/w/cpp/concepts/semiregular
//
// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl_test/base/concept/copyable.hpp>

namespace ac {

template <class T>
void test_semiregular_type_traits()
{
    test_copyable_type_traits<T>();
    static_assert(std::is_default_constructible_v<T>);
}

template <class T>
void test_trivially_semiregular_type_traits()
{
    test_trivially_copyable_type_traits<T>();
    static_assert(std::is_trivially_default_constructible_v<T>);
    // is_trivial is defined as trivially copyable and default constructible,
    // but it can have different internal implementation by a compiler.
    static_assert(std::is_trivial_v<T>);
}

template <class T>
void test_nothrow_semiregular_type_traits()
{
    test_nothrow_copyable_type_traits<T>();
    static_assert(std::is_nothrow_default_constructible_v<T>);
}

} // namespace ac
