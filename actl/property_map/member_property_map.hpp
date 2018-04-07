/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>

namespace ac {

namespace detail {

template <class Key, class Member>
using member_pm_base = property_map<Key&, Member, Member&, false, false,
                                    !std::is_const_v<Key> && !std::is_const_v<Member>>;
}

/**
 * Property map that fetches member from a class key.
 */
template <class Key, class Member>
class member_property_map : public detail::member_pm_base<Key, Member> {
public:
    explicit constexpr member_property_map(Member Key::* ptr) : ptr_{ptr} {}

    friend Member& get(member_property_map& mp, Key& key) { return key.*mp.ptr_; }

    template <bool W = detail::member_pm_base<Key, Member>::writeable>
    friend std::enable_if_t<W> put(member_property_map& mp, Key& key, Member value) {
        key.*mp.ptr_ = value;
    }

    // TODO: consider making this invertible. It's possible, but not standard-compliant.

private:
    Member Key::* ptr_;
};

template <class Key, class Member>
inline auto make_member_property_map(Member Key::* ptr) {
    return member_property_map<Key, Member>(ptr);
}

template <class Key, class Member>
inline auto make_const_member_property_map(Member Key::* ptr) {
    return member_property_map<const Key, const Member>(ptr);
}

/**
 * Member property map with member mapping known at compile-time.
 */
template <class Key, class Member, Member Key::* Ptr>
class static_member_property_map : public detail::member_pm_base<Key, Member> {
public:
    friend Member& get(static_member_property_map& mp, Key& key) { return key.*Ptr; }

    template <bool W = detail::member_pm_base<Key, Member>::writeable>
    friend std::enable_if_t<W> put(static_member_property_map& mp, Key& key, Member value) {
        key.*Ptr = value;
    }
};

}  // namespace ac
