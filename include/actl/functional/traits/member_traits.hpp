// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/traits/MemberFunction.hpp>
#include <actl/functional/traits/parameters.hpp>
#include <actl/meta/qualifiers/copy.hpp>

namespace ac {

/// Class parameter of the member function including qualifiers.
template<MemberFunction MF>
using class_parameter_t = at_t<parameters_t<MF>, 0>;

template<typename T>
struct class_of;
/// Class corresponding to a member function or member data pointer
/// excluding qualifiers.
template<typename T>
using class_of_t = typename class_of<T>::type;

template<typename T, typename Class>
struct as_member_of {
    using type = T Class::*;
};
/// Resulting type is different for different input types T:
/// - If T is a free function, then it's a member function of Class
///   with the same parameters and return type.
/// - If T is a member function, then it's a member function of Class
///   with the same member qualifiers, parameters and return type.
/// - Otherwise, it's a member data pointer `T Class::*`.
template<typename T, typename Class>
using as_member_of_t = typename as_member_of<T, Class>::type;

/* implementation */

template<MemberFunction MF>
struct class_of<MF> {
    using type = std::remove_cvref_t<class_parameter_t<MF>>;
};
template<typename Class, typename Member>
struct class_of<Member Class::*> {
    using type = Class;
};

namespace detail {

template<MemberFunction MF>
using unique_class_t =
    at_t<typename function_traits<MF>::unique_parameters_type, 0>;

template<
    MemberFunction MF,
    template<typename> typename Modifier,
    typename ParameterList>
struct modify_class;

template<
    MemberFunction MF,
    template<typename> typename Modifier,
    typename Class,
    typename... Parameters>
struct modify_class<MF, Modifier, type_list<Class, Parameters...>> {
    using Traits = function_traits<MF>;

    using type = assemble_function_t<
        function_category::member,
        typename Traits::return_type,
        type_list<typename Modifier<Class>::type, Parameters...>,
        Traits::accepts_variadic_arguments,
        Traits::is_noexcept>;
};

template<MemberFunction MF, template<typename> typename Modifier>
using modify_class_t =
    typename modify_class<MF, Modifier, unique_parameters_t<MF>>::type;

} // namespace detail

template<FreeFunction FF, typename Class>
struct as_member_of<FF, Class> {
    using traits = function_traits<FF>;

    using type = assemble_function_t<
        function_category::member,
        typename traits::return_type,
        concat_t<type_list<Class>, typename traits::parameters_type>,
        traits::accepts_variadic_arguments,
        traits::is_noexcept>;
};

template<MemberFunction MF, typename Class>
struct as_member_of<MF, Class> {
    template<typename T>
    struct copy_cvref_from {
        using type = copy_cvref_t<Class, T>;
    };

    using type = detail::modify_class_t<MF, copy_cvref_from>;
};

} // namespace ac
