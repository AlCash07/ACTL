/***************************************************************************************************
 * Standard containers traits and categorization.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/none.hpp>
#include <list>
#include <map>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace ac {

/* Container tags */

struct container_tag {};

struct sequence_tag : container_tag {};
struct random_access_tag : sequence_tag {};

struct associative_tag : container_tag {};
struct unique_associative_tag : associative_tag {};
struct multiple_associative_tag : associative_tag {};

template <class C>
struct container_traits;

template <class C>
struct container_traits<const C> : container_traits<C> {};

template <class T>
struct container_traits<std::vector<T>> {
    using category = random_access_tag;

    template <class U>
    using rebind = std::vector<U>;
};

template <class T>
struct container_traits<std::list<T>> {
    using category = sequence_tag;

    template <class U>
    using rebind = std::list<U>;
};

template <class T>
struct container_traits<std::set<T>> {
    using category = unique_associative_tag;

    template <class U>
    using rebind = std::set<U>;
};

template <class T>
struct container_traits<std::multiset<T>> {
    using category = multiple_associative_tag;

    template <class U>
    using rebind = std::multiset<U>;
};

template <class T>
struct container_traits<std::unordered_set<T>> {
    using category = unique_associative_tag;

    template <class U>
    using rebind = std::unordered_set<U>;
};

template <class T>
struct container_traits<std::unordered_multiset<T>> {
    using category = multiple_associative_tag;

    template <class U>
    using rebind = std::unordered_multiset<U>;
};

template <>
struct container_traits<none> {
    template <class>
    using rebind = none;
};

/* Container type traits */

namespace detail {

template <class CT, class = void>
struct is_container_impl : std::false_type {};

template <class CT>
struct is_container_impl<CT, std::void_t<typename CT::category>> : std::true_type {};

template <class C, class Tag, bool = true>
struct has_container_tag_impl : std::is_base_of<Tag, typename container_traits<C>::category> {};

template <class C, class Tag>
struct has_container_tag_impl<C, Tag, false> : std::false_type {};

}  // namespace detail

template <class C>
struct is_container : detail::is_container_impl<container_traits<C>> {};

template <class C>
inline constexpr bool is_container_v = is_container<C>::value;

template <class C, class Tag>
struct has_container_tag : detail::has_container_tag_impl<C, Tag, is_container<C>::value> {};

template <class C> struct is_sequence             : has_container_tag<C, sequence_tag> {};
template <class C> struct is_random_access        : has_container_tag<C, random_access_tag> {};
template <class C> struct is_associative          : has_container_tag<C, associative_tag> {};
template <class C> struct is_unique_associative   : has_container_tag<C, unique_associative_tag> {};
template <class C> struct is_multiple_associative : has_container_tag<C, multiple_associative_tag> {
};

template <class C> inline constexpr bool is_random_access_v = is_random_access<C>::value;
template <class C> inline constexpr bool is_associative_v   = is_associative<C>::value;

// Does erasure invalidate container iterators except the erased one.
template <class C>
struct is_stable : std::bool_constant<!is_random_access_v<C>> {};

template <class C> struct value_type       { using type = typename C::value_type; };
template <>        struct value_type<none> { using type = none; };

template <class C>
using value_type_t = typename value_type<C>::type;

}  // namespace ac
