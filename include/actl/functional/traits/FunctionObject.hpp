// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/MemberFunction.hpp>
#include <actl/meta/type_list/slice.hpp>

namespace ac {

/// Concept of a function object, that is a type with
/// a non-overloaded `operator()`.
template<class T>
concept FunctionObject = requires(T) {
    &T::operator();
    // TODO: investigate whether type qualifiers on Fn can make it
    // impossible to call `operator()`, and, on the other hand, may
    // disambiguate it.
};

template<FunctionObject Fn>
struct function_traits<Fn>
    : function_traits<decltype(&std::remove_reference_t<Fn>::operator())> {
private:
    using all_parameters = typename function_traits<
        decltype(&std::remove_reference_t<Fn>::operator())>::parameter_types;

public:
    static constexpr auto category = function_category::object;
    // Excluding the class parameter.
    using parameter_types = slice_t<all_parameters, 1, all_parameters::length>;
};

} // namespace ac
