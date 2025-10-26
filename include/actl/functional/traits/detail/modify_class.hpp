// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/traits/MemberFunction.hpp>
#include <actl/functional/traits/parameters.hpp>

namespace ac::detail {

template<MemberFunction MF>
using unique_class_t =
    at_t<typename function_traits<MF>::unique_parameters_type, 0>;

template<MemberFunction MF, template<class> class Modifier, class ParameterList>
struct modify_class;

template<
    MemberFunction MF,
    template<class>
    class Modifier,
    class Class,
    class... Parameters>
struct modify_class<MF, Modifier, type_list<Class, Parameters...>> {
    using Traits = function_traits<MF>;

    using type = assemble_function_t<
        function_category::member,
        typename Traits::return_type,
        type_list<typename Modifier<Class>::type, Parameters...>,
        Traits::accepts_variadic_arguments,
        Traits::is_noexcept>;
};

template<MemberFunction MF, template<class> class Modifier>
using modify_class_t =
    typename modify_class<MF, Modifier, unique_parameters_t<MF>>::type;

} // namespace ac::detail
