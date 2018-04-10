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
 *
 * If a property map incapsulates a container or a reference container, use property_map_container
 * wrapper to deal with constness.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/type_traits/is_iterator.hpp>
#include <actl/type_traits/type_traits.hpp>

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

namespace detail {

template <class T, bool Map = false, bool Iterator = false>
struct property_traits_impl {};

template <class T>
struct property_traits_impl<T, true, false> {
    using key_type   = typename T::key_type;
    using value_type = typename T::value_type;
    using reference  = typename T::reference;

    static constexpr bool invertible = T::invertible;
    static constexpr bool iterable   = T::iterable;
    static constexpr bool writable   = T::writable;
};

template <class T>
struct property_traits_impl<T, false, true> {
    using key_type   = int;
    using value_type = typename std::iterator_traits<T>::value_type;
    using reference  = typename std::iterator_traits<T>::reference;

    // TODO: if invert is guaranteed to take the result of get then invertible is true.
    static constexpr bool invertible = false;
    static constexpr bool iterable   = false;
    static constexpr bool writable   = is_non_const_reference<reference>::value;
};

template <class C, bool Mutable>
struct pm_container {
    C& operator()() const { return data_; }

    mutable C data_;
};

template <class C>
struct pm_container<C, false> {
    C& operator()() const { return data_; }

    C data_;
};

}  // namespace detail

template <class T>
struct property_traits : detail::property_traits_impl<T, std::is_base_of_v<property_map_base, T>,
                                                      is_random_access_iterator<T>::value> {};

template <class Container, class C = std::remove_reference_t<Container>,
          bool M = !std::is_const_v<C>>
struct property_map_container : detail::pm_container<Container, !std::is_reference_v<Container>> {
    using reference = typename C::reference;
    using iterator  = typename C::iterator;

    static constexpr bool writable = true;
};

template <class Container, class C>
struct property_map_container<Container, C, false> : detail::pm_container<Container, false> {
    using reference = typename C::const_reference;
    using iterator  = typename C::const_iterator;

    static constexpr bool writable = false;
};

template <class It>
inline std::enable_if_t<is_random_access_iterator<It>::value,
                        typename std::iterator_traits<It>::reference>
get(It it, int key) {
    return it[key];
}

template <class It>
inline std::enable_if_t<is_random_access_iterator<It>::value> put(
    It it, int key, typename std::iterator_traits<It>::value_type value) {
    return it[key] = value;
}

}  // namespace ac
