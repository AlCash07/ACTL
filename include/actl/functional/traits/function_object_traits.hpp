// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/member_function_traits.hpp>

namespace ac {

/// Callable traits specialized only for function objects.
template<class T>
struct function_object_traits {
    static constexpr bool is_function_object = false;
};

/// Concept of a function object, that is a type with
/// a non-overloaded `operator()`.
template<class T>
concept FunctionObject = function_object_traits<T>::is_function_object;

template<class Fn>
    requires requires(Fn) {
        &Fn::operator();
        // TODO: investigate whether type qualifiers on Fn can make it
        // impossible to call `operator()`, and, on the other hand, may
        // disambiguate it.
    }
struct function_object_traits<Fn> {
private:
    using Traits = member_function_traits<decltype(&Fn::operator())>;

public:
    using return_type = typename Traits::return_type;

    static constexpr size_t arity = Traits::arity - 1;

    template<size_t Index>
    using parameter_at = typename Traits::template parameter_at<Index + 1>;

    static constexpr bool is_noexcept = Traits::is_noexcept;

    static constexpr bool is_function_object = true;
};

} // namespace ac
