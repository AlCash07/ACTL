// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/traits/MemberFunction.hpp>
#include <actl/meta/qualifiers/copy.hpp>

namespace ac {

/// Class parameter of the member function including qualifiers.
template<MemberFunction MF>
using class_parameter_t = at_t<parameters_t<MF>, 0>;

/// Class parameter of the member function excluding qualifiers.
template<MemberFunction MF>
using unqualified_class_t = std::remove_cvref_t<at_t<parameters_t<MF>, 0>>;

template<class T, class Class>
struct as_member_of {
    using type = T Class::*;
};
/// Resulting type is different for different input types T:
/// - If T is a free function, then it's a member function of Class
///   with the same parameters and return type.
/// - If T is a member function, then it's a member function of Class
///   with the same member qualifiers, parameters and return type.
/// - Otherwise, it's a member data pointer `T Class::*`.
template<class T, class Class>
using as_member_of_t = typename as_member_of<T, Class>::type;

/* implementation */

template<FreeFunction FF, class Class>
struct as_member_of<FF, Class> {
    using traits = function_traits<FF>;

    using type = assemble_function_t<
        function_category::member,
        typename traits::return_type,
        concat_t<type_list<Class>, typename traits::parameters_type>,
        traits::accepts_variadic_arguments,
        traits::is_noexcept>;
};

template<MemberFunction MF, class Class>
struct as_member_of<MF, Class> {
    template<class T>
    struct copy_cvref_from {
        using type = copy_cvref_t<Class, T>;
    };

    using type = detail::modify_class_t<MF, copy_cvref_from>;
};

} // namespace ac
