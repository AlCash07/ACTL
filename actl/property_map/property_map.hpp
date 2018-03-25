/***************************************************************************************************
 * Property maps inspired by boost, but much more powerful.
 * Besides mapping with operator [], property maps can support:
 * - inverse mapping (invert method);
 * - traversal of container maps (begin(), end() methods).
 *
 * Custom property maps must inherit from property_map_base, which is done more conveniently by
 * inheriting from one of:
 * - property_map, which defines all required nested types and static variables;
 * - computed_property_map, where value is assumed to be computed on the fly by a function;
 * - container_property_map, which incapsulates a container (or a reference to container) and
 * provides traversal interface.
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
#include <iterator>

namespace ac {

struct property_map_base {};

template <bool Traversible,
          bool Invertible,
          class Key,
          class Value,
          class Ref  = Value&,
          class CRef = const Value&>
struct property_map : property_map_base {
    using key_type        = Key;
    using value_type      = Value;
    using reference       = Ref;
    using const_reference = CRef;

    static constexpr bool writeable   = is_non_const_reference<reference>::value;
    static constexpr bool traversible = Traversible;
    static constexpr bool invertible  = Invertible;
};

template <bool Invertible, class Key, class Ref>
struct computed_property_map : property_map<false, Invertible, Key, std::decay_t<Ref>, Ref, Ref> {
    void clear() {}
};

template <bool Invertible, class Container, class Key, class Value, class Ref, class CRef>
class container_property_map : public property_map<true, Invertible, Key, Value, Ref, CRef> {
public:
    using iterator       = typename remove_reference_t<Container>::iterator;
    using const_iterator = typename remove_reference_t<Container>::const_iterator;

    template <class... Ts>
    explicit container_property_map(Ts&&... args) : data_(std::forward<Ts>(args)...) {}

    iterator begin() { return data_.begin(); }
    iterator end()   { return data_.end(); }

    const_iterator begin() const { return data_.begin(); }
    const_iterator end()   const { return data_.end(); }

    void clear() { data_.clear(); }

    void swap(container_property_map& other) { data_.swap(other.data_); }

protected:
    Container data_;
};

namespace detail {

template <class T, bool Map = false, bool Iterator = false>
struct property_traits_impl {};

template <class T>
struct property_traits_impl<T, true, false> {
    using key_type        = typename T::key_type;
    using value_type      = typename T::value_type;
    using reference       = typename T::reference;
    using const_reference = typename T::const_reference;

    static constexpr bool writeable   = T::writeable;
    static constexpr bool traversible = T::traversible;
    static constexpr bool invertible  = T::invertible;
};

template <class T>
struct property_traits_impl<T, false, true> {
    using key_type        = typename std::iterator_traits<T>::difference_type;
    using value_type      = typename std::iterator_traits<T>::value_type;
    using reference       = typename std::iterator_traits<T>::reference;
    using const_reference = const reference;

    static constexpr bool writeable   = is_non_const_reference<reference>::value;
    static constexpr bool traversible = false;
    static constexpr bool invertible  = false;
};

}  // namespace detail

template <class T>
struct property_traits : detail::property_traits_impl<T, std::is_base_of_v<property_map_base, T>,
                                                      is_random_access_iterator<T>::value> {};

}  // namespace ac
