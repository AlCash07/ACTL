/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/map.hpp>

namespace ac {

/**
 * Property map that fetches member from a class key.
 */
// TODO: consider making this map invertible. It's possible, but not standard-compliant.
template <class Class, class Member>
class member_map {
public:
    using key_type = Class&;
    using reference = Member&;

    explicit constexpr member_map(Member Class::* ptr) : ptr{ptr} {}

    Member Class::* const ptr;
};

/**
 * Member property map with member mapping known at compile-time.
 */
template <auto Ptr>
class static_member_map;

template <class Class, class Member, Member Class::* Ptr>
class static_member_map<Ptr> {
public:
    using key_type = Class&;
    using reference = Member&;

    static constexpr auto ptr = Ptr;
};

namespace detail {

template <class T>
struct is_memmap : std::false_type {};

template <class C, class M>
struct is_memmap<member_map<C, M>> : std::true_type {};

template <auto Ptr>
struct is_memmap<static_member_map<Ptr>> : std::true_type {};

}  // namespace detail

template <class Map>
struct map_types<const Map, std::enable_if_t<detail::is_memmap<Map>::value>> {
    template <class T>
    using make_const_ref_t = const std::remove_reference_t<T>&;

    using key_type = make_const_ref_t<typename Map::key_type>;
    using reference = make_const_ref_t<typename Map::reference>;
};

template <class Map, std::enable_if_t<detail::is_memmap<std::remove_const_t<Map>>::value, int> = 0>
inline typename map_types<Map>::reference get(Map& map, typename map_types<Map>::key_type key) {
    return key.*map.ptr;
}

}  // namespace ac
