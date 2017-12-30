/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>

namespace ac {

/**
 * Property map that fetches member from a class key.
 */
template <class Key, class Member>
class member_property_map : public property_map<false, false, Key, Member> {
public:
    explicit constexpr member_property_map(Member Key::* ptr) : ptr_{ptr} {}

    constexpr Member& operator[](Key& key) { return key.*ptr_; }

    constexpr const Member& operator[](const Key& key) const { return key.*ptr_; }

private:
    Member Key::* ptr_;
};

template <class Key, class Member>
inline auto make_member_property_map(Member Key::* ptr) {
    return member_property_map<Key, Member>(ptr);
}

/**
 * Member property map with member mapping known at compile-time.
 */
template <class Key, class Member, Member Key::* Ptr>
class static_member_property_map : public property_map<false, false, Key, Member> {
public:
    constexpr Member& operator[](Key& key) { return key.*Ptr; }

    constexpr const Member& operator[](const Key& key) const { return key.*Ptr; }
};

}  // namespace ac
