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
    static constexpr auto ptr = Ptr;
};

template <class C, class M>
struct map_types<member_map<C, M>> {
    using key_type = C&;
    using reference = M&;

    struct member_map_tag;
};

template <class C, class M, M C::* Ptr>
struct map_types<static_member_map<Ptr>> {
    using key_type = C&;
    using reference = M&;

    struct member_map_tag;
};

namespace detail {

template <class T, class = void>
struct is_memmap : std::false_type {};

template <class T>
struct is_memmap<T, std::void_t<typename map_types<T>::member_map_tag>> : std::true_type {};

}  // namespace detail

template <class Map>
struct map_types<const Map, std::enable_if_t<detail::is_memmap<Map>::value>> {
    template <class T>
    using make_const_ref_t = const std::remove_reference_t<T>&;

    using key_type = make_const_ref_t<map_key_t<Map>>;
    using reference = make_const_ref_t<map_reference_t<Map>>;
};

template <class Map, std::enable_if_t<detail::is_memmap<std::remove_const_t<Map>>::value, int> = 0>
inline map_reference_t<Map> get(Map& map, map_key_t<Map> key) {
    return key.*map.ptr;
}

}  // namespace ac
