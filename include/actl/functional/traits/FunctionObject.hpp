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
/// a non-overloaded `operator()`, for example
/// @code
/// template<typename T>
/// struct plus {
///     constexpr T operator()(T const& l, T const& r) const {
///         return l + r;
///     }
/// };
/// @endcode
/// Non-template lambdas are also function objects.
template<typename T>
concept FunctionObject = requires(T) {
    &T::operator();
    // TODO: investigate whether type qualifiers on T can make it
    // impossible to call `operator()`, and, on the other hand, may
    // disambiguate it.
};

/* Implementation */

template<FunctionObject Function>
struct function_traits<Function> {
private:
    using member_traits =
        function_traits<decltype(&std::remove_reference_t<Function>::operator()
        )>;
    using all_parameters = typename member_traits::parameters_type;

public:
    static constexpr auto category = function_category::object;
    using return_type = typename member_traits::return_type;
    // class parameter is the function object itself, so it should be excluded
    using parameters_type = slice_t<all_parameters, 1, all_parameters::length>;
    static constexpr bool accepts_variadic_arguments =
        member_traits::accepts_variadic_arguments;
    static constexpr bool is_noexcept = member_traits::is_noexcept;
};

} // namespace ac
