/***************************************************************************************************
 * Property maps inspired by boost, but much more powerful.
 * Besides access via get and put functions, property maps can support:
 * - inverse mapping (invert method);
 * - traversal of container maps (begin(), end() methods);
 * - clear method (not formalized yet).
 *
 * Custom property maps must inherit from property_map_base, which is done more conveniently by
 * inheriting from property_map, which defines all required nested types and static variables;
 * the last template parameters have suitable defaults for the case when mapped value is computed on
 * the fly.
 *
 * property_traits is a helper class that defines nested types and static variables for custom
 * property maps as well as for random access iterators (which can be used as property maps from
 * integer domain).
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/iterator_traits.hpp>
#include <actl/traits/type_traits.hpp>

namespace ac {

struct property_map_base {};

template <class Key, class Value, class Ref, bool Invertible, bool Iterable = false,
          bool Writable = false>
struct property_map : property_map_base {
    using key_type   = Key;
    using value_type = Value;
    using reference  = Ref;

    static constexpr bool invertible = Invertible;
    static constexpr bool iterable   = Iterable;
    static constexpr bool writable   = Writable;
};

template <class It>
class iterator_property_map : property_map_base {
    It it_;

public:
    static_assert(is_random_access_iterator_v<It>);

    using key_type   = int;
    using value_type = typename std::iterator_traits<It>::value_type;
    using reference  = typename std::iterator_traits<It>::reference;
    using wrapper    = iterator_property_map<It>;

    // TODO: if invert is guaranteed to take the result of get then invertible can be true.
    static constexpr bool invertible = false;
    static constexpr bool iterable   = false;
    static constexpr bool writable   = std::is_assignable_v<reference, value_type>;

    constexpr iterator_property_map(It it) : it_{it} {}

    constexpr operator It() const { return it_; }

    friend reference get(const iterator_property_map& pm, key_type key) { return pm[key]; }
};

namespace detail {

template <class PM, bool IsMap = true>
struct property_traits_impl {
    using key_type   = typename PM::key_type;
    using value_type = typename PM::value_type;
    using reference  = typename PM::reference;
    using wrapper    = PM;

    static constexpr bool invertible = PM::invertible;
    static constexpr bool iterable   = PM::iterable;
    static constexpr bool writable   = PM::writable;
};

template <class It>
struct property_traits_impl<It, false> : property_traits_impl<iterator_property_map<It>> {};

}  // namespace detail

template <class PM>
struct property_traits
    : detail::property_traits_impl<PM, std::is_base_of_v<property_map_base, PM>> {};

// This type can be used as base class even when PM is a pointer.
template <class PM>
using property_map_wrapper_t = typename property_traits<PM>::wrapper;

template <class It>
inline std::enable_if_t<is_random_access_iterator_v<It>, typename property_traits<It>::reference>
get(const It& it, int key) {  // const It& disallows conversion from array to pointer.
    return it[key];
}

// Default put implementation.
template <class PM>
inline std::enable_if_t<property_traits<PM>::writable> put(
    const PM& pm, typename property_traits<PM>::key_type key,
    typename property_traits<PM>::value_type value) {
    get(pm, key) = value;
}

}  // namespace ac
