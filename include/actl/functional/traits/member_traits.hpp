// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/traits/detail/modify_class.hpp>
#include <actl/meta/qualifiers/inner.hpp>

namespace ac {

template<MemberFunction MF>
using class_parameter_t = at_t<parameters_t<MF>, 0>;

/* const */

template<MemberFunction MF>
inline constexpr bool is_const_member_v =
    std::is_const_v<std::remove_reference_t<detail::unique_class_t<MF>>>;

template<MemberFunction MF>
using add_member_const_t = detail::modify_class_t<MF, add_inner_const>;

template<MemberFunction MF>
using remove_member_const_t = detail::modify_class_t<MF, remove_inner_const>;

/* reference */

template<MemberFunction MF>
inline constexpr bool is_reference_member_v =
    std::is_reference_v<detail::unique_class_t<MF>>;

template<MemberFunction MF>
using remove_member_reference_t =
    detail::modify_class_t<MF, std::remove_reference>;

template<MemberFunction MF>
inline constexpr bool is_lvalue_reference_member_v =
    std::is_lvalue_reference_v<detail::unique_class_t<MF>>;

template<MemberFunction MF>
using add_member_lvalue_reference_t =
    detail::modify_class_t<MF, std::add_lvalue_reference>;

template<MemberFunction MF>
inline constexpr bool is_rvalue_reference_member_v =
    std::is_rvalue_reference_v<detail::unique_class_t<MF>>;

template<MemberFunction MF>
using add_member_rvalue_reference_t =
    detail::modify_class_t<MF, std::add_rvalue_reference>;

/* volatile */

template<MemberFunction MF>
inline constexpr bool is_volatile_member_v =
    std::is_volatile_v<std::remove_reference_t<detail::unique_class_t<MF>>>;

template<MemberFunction MF>
using add_member_volatile_t = detail::modify_class_t<MF, add_inner_volatile>;

template<MemberFunction MF>
using remove_member_volatile_t =
    detail::modify_class_t<MF, remove_inner_volatile>;

/* cv - const volatile */

template<MemberFunction MF>
inline constexpr bool is_cv_member_v =
    is_const_member_v<MF> && is_volatile_member_v<MF>;

template<MemberFunction MF>
using add_member_cv_t = detail::modify_class_t<MF, add_inner_cv>;

template<MemberFunction MF>
using remove_member_cv_t = detail::modify_class_t<MF, remove_inner_cv>;

} // namespace ac
