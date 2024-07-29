// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/category/function_object.hpp>
#include <actl/functional/traits/detail/member_function.hpp>

namespace ac::detail {

template<class Fn>
    requires FunctionObject<std::remove_reference_t<Fn>>
struct function_traits<Fn> {
private:
    using operator_traits =
        function_traits<decltype(&std::remove_reference_t<Fn>::operator())>;

public:
    using return_type = typename operator_traits::return_type;

    static constexpr size_t arity = operator_traits::arity - 1;

    template<size_t Index>
    using parameter_at =
        typename operator_traits::template parameter_at<Index + 1>;

    static constexpr bool is_noexcept = operator_traits::is_noexcept;
};

} // namespace ac::detail
