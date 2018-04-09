/***************************************************************************************************
 * Property maps inspired by boost, but much more powerful.
 * Besides access via get and put functions, property maps can support:
 * - inverse mapping (invert method);
 * - traversal of container maps (begin(), end() methods);
 * - clear method (not formalized yet).
 *
 * Custom property maps must inherit from property_map_base, which is done more conveniently by
 * inheriting from one of:
 * - property_map, which defines all required nested types and static variables; the last template
 * parameters have suitable defaults for the case when mapped value is computed on the fly.
 * - container_property_map, which incapsulates a container (or a reference to container,
 * also can be constant) and provides traversal interface.
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

#include <actl/type_traits/is_iterator.hpp>
#include <actl/type_traits/type_traits.hpp>

namespace ac {

struct property_map_base {};

template <class Key, class Value, class Ref, bool ConstGet, bool Writable, bool Invertible,
          bool Iterable>
struct property_map : property_map_base {
    using key_type   = Key;
    using value_type = Value;
    using reference  = Ref;

    static constexpr bool const_get  = ConstGet;
    static constexpr bool writable   = Writable;
    static constexpr bool invertible = Invertible;
    static constexpr bool iterable   = Iterable;
};

template <class Key, class Value, class Ref, bool Invertible>
using computed_property_map = property_map<Key, Value, Ref, true, false, Invertible, false>;

template <class Container, class Key, class Value, class Ref, bool Invertible,
          bool Writable = !std::is_const_v<std::remove_reference_t<Container>>>
class container_property_map : public property_map<Key, Value, Ref, true, Invertible, true, Writable> {
    using C = std::remove_reference_t<Container>;

public:
    using iterator = std::conditional_t<Writable, typename C::iterator, typename C::const_iterator>;

    template <class... Ts>
    explicit container_property_map(Ts&&... args) : data_(std::forward<Ts>(args)...) {}

    iterator begin() { return data_.begin(); }
    iterator end()   { return data_.end(); }

protected:
    Container data_;
};

namespace detail {

template <class T, bool Map = false, bool Iterator = false>
struct property_traits_impl {};

template <class T>
struct property_traits_impl<T, true, false> {
    using key_type   = typename T::key_type;
    using value_type = typename T::value_type;
    using reference  = typename T::reference;

    static constexpr bool const_get  = T::const_get;
    static constexpr bool writable   = T::writable;
    static constexpr bool invertible = T::invertible;
    static constexpr bool iterable   = T::iterable;
};

template <class T>
struct property_traits_impl<T, false, true> {
    using key_type   = int;
    using value_type = typename std::iterator_traits<T>::value_type;
    using reference  = typename std::iterator_traits<T>::reference;

    static constexpr bool const_get  = true;
    static constexpr bool writable   = is_non_const_reference<reference>::value;
    // TODO: if invert is guaranteed to take the result of get then invertible is true.
    static constexpr bool invertible = false;
    static constexpr bool iterable   = false;
};

}  // namespace detail

template <class T>
struct property_traits : detail::property_traits_impl<T, std::is_base_of_v<property_map_base, T>,
                                                      is_random_access_iterator<T>::value> {};

template <class It>
inline std::enable_if_t<is_random_access_iterator<It>::value, typename std::iterator_traits<It>::reference>
get(It it, int key) {
    return it[key];
}

template <class It>
inline std::enable_if_t<is_random_access_iterator<It>::value> put(
    It it, int key, typename std::iterator_traits<It>::value_type value) {
    return it[key] = value;
}

template <class PM, class Base, bool W = Base::writable>
struct put_gen : Base {
    using typename Base::key_type;
    using typename Base::value_type;

    friend constexpr void put(put_gen& pm, key_type key, value_type value) {
        return static_cast<PM&>(pm).put(key, value);
    }

    friend constexpr void put(put_gen&& pm, key_type key, value_type value) {
        return put(pm, key, value);
    }
};

template <class PM, class Base>
struct put_gen<PM, Base, false> : Base {};

template <class PM, class Base, bool CG = Base::const_get>
struct get_put_gen : put_gen<PM, Base> {
    friend constexpr typename Base::reference get(const get_put_gen& pm, typename Base::key_type key) {
        return static_cast<const PM&>(pm).get(key);
    }
};

template <class PM, class Base>
struct get_put_gen<PM, Base, false> : put_gen<PM, Base> {
    using typename Base::key_type;
    using typename Base::reference;

    friend constexpr reference get(get_put_gen& pm, key_type key) {
        return static_cast<PM&>(pm).get(key);
    }

    friend constexpr reference get(get_put_gen&& pm, key_type key) {
        return get(pm, key);
    }
};

}  // namespace ac
