// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/traits.hpp>

namespace ac {

/// Property map that fetches member from a class key.
// TODO: consider making this map invertible. Iter's possible, but not standard-compliant.
template <class Class, class Member>
class member_map {
public:
    using class_t = Class;
    using member_t = Member;

    explicit constexpr member_map(Member Class::*ptr) : ptr{ptr} {}

    Member Class::*const ptr;
};

/// Member property map with member mapping known at compile-time.
template <auto Ptr>
class static_member_map;

template <class Class, class Member, Member Class::*Ptr>
class static_member_map<Ptr> {
public:
    using class_t = Class;
    using member_t = Member;

    static constexpr auto ptr = Ptr;
};

template <class MM>
struct map_traits<MM, std::void_t<typename MM::class_t, typename MM::member_t>> {
    static constexpr bool C = std::is_const_v<MM>;

    using key_type = add_const_if_t<C, typename MM::class_t>&;
    using value_type = typename MM::member_t;
    using reference = add_const_if_t<C, value_type>&;

    static constexpr bool readable = true;
    static constexpr bool writable = !C && !std::is_const_v<value_type>;
    static constexpr bool invertible = false;
    static constexpr bool iterable = false;
};

template <class MM>
struct map_ops<MM, std::void_t<typename MM::class_t, typename MM::member_t>> : map_put<MM> {
    static map_reference_t<MM> get(MM& map, map_key_t<MM> key) {
        return key.*map.ptr;
    }
};

}  // namespace ac
