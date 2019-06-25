/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/property_map.hpp>

namespace ac {

namespace detail {

template <class Key, class Member>
using member_map_base = property_map<Key&, Member, Member&, false, false,
                                     !std::is_const_v<Key> && !std::is_const_v<Member>>;

}  // namespace detail

/**
 * Property map that fetches member from a class key.
 */
template <class Key, class Member>
class member_map : public detail::member_map_base<Key, Member> {
public:
    explicit constexpr member_map(Member Key::* ptr) : ptr_{ptr} {}

    friend Member& get(member_map map, Key& key) { return key.*map.ptr_; }

    // TODO: consider making this invertible. It's possible, but not standard-compliant.

private:
    Member Key::*ptr_;
};

template <class Key, class Member>
inline auto const_member_property_map(Member Key::* ptr) {
    return member_map<const Key, const Member>{ptr};
}

/**
 * Member property map with member mapping known at compile-time.
 */
template <auto Ptr>
class static_member_map;

template <class Key, class Member, Member Key::* Ptr>
class static_member_map<Ptr> : public detail::member_map_base<Key, Member> {
public:
    friend Member& get(static_member_map, Key& key) { return key.*Ptr; }
};

template <auto Ptr>
class static_const_member_map;

template <class Key, class Member, Member Key::* Ptr>
class static_const_member_map<Ptr> : public detail::member_map_base<const Key, const Member> {
public:
    friend const Member& get(static_const_member_map, const Key& key) { return key.*Ptr; }
};

}  // namespace ac