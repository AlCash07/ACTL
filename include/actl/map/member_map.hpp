/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/traits.hpp>

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

namespace detail {

struct memmap_tag {};

template <class T>
inline constexpr bool is_memmap_v =
    !std::is_reference_v<T> && std::is_base_of_v<memmap_tag, map_traits<T>>;

template <class T>
using cref_t = const std::remove_reference_t<T>&;

}  // namespace detail

template <class C, class M>
struct map_traits<member_map<C, M>> : map_traits_base<C&, M&>, detail::memmap_tag {};

template <class C, class M, M C::* Ptr>
struct map_traits<static_member_map<Ptr>> : map_traits_base<C&, M&>, detail::memmap_tag {};

template <class MM>
struct map_traits<const MM, std::enable_if_t<detail::is_memmap_v<MM>>>
    : map_traits_base<detail::cref_t<map_key_t<MM>>, detail::cref_t<map_reference_t<MM>>> {};

template <class MM>
struct map_ops<MM, std::enable_if_t<detail::is_memmap_v<std::remove_const_t<MM>>>> {
    static map_reference_t<MM> get(MM& map, map_key_t<MM> key) { return key.*map.ptr; }
};

}  // namespace ac
