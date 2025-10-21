// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/traits/FunctionObject.hpp>
#include <actl/functional/traits/detail/member_function.hpp>
#include <actl/meta/type_list/slice.hpp>

namespace ac::detail {

template<class Fn>
    requires FunctionObject<std::remove_reference_t<Fn>>
struct function_traits<Fn>
    : function_traits<decltype(&std::remove_reference_t<Fn>::operator())> {
private:
    using full_parameters = typename function_traits<
        decltype(&std::remove_reference_t<Fn>::operator())>::parameter_types;

public:
    using parameter_types =
        slice_t<full_parameters, 1, full_parameters::length>;
};

} // namespace ac::detail
