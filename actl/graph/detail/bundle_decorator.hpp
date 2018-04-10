/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/composite_property_map.hpp>
#include <actl/type_traits/type_traits.hpp>

namespace ac::detail {

// Special compressed_pair that decorates T with bundled property B.
template <class T, class B, bool = std::is_empty_v<B> && !std::is_final_v<B>>
class bundle_decorator : public T, private B {
public:
    using bundle_type = B;

    bundle_decorator() = default;

    template <class T0, class... Ts>
    explicit constexpr bundle_decorator(T0&& arg0, Ts&&... args)
        : T(std::forward<T0>(arg0)), B(std::forward<Ts>(args)...) {}

    B&       bundle()       { return *this; }
    const B& bundle() const { return *this; }
};

template <class T, class B>
class bundle_decorator<T, B, false> : public T {
public:
    using bundle_type = B;

    bundle_decorator() = default;

    template <class T0, class... Ts>
    explicit constexpr bundle_decorator(T0&& arg0, Ts&&... args)
        : T(std::forward<T0>(arg0)), bundle_(std::forward<Ts>(args)...) {}

    B&       bundle()       { return bundle_; }
    const B& bundle() const { return bundle_; }

private:
    B bundle_;
};

template <class Key, class B = typename std::remove_reference_t<Key>::bundle_type,
          bool W = !std::is_const_v<Key>>
class bundle_property_map : public property_map<Key, B, add_const_if_t<!W, B&>, false, false, W> {
    friend add_const_if_t<W, B&> get(const bundle_property_map& pm, Key key) {
        return key.bundle();
    }
};

template <class Key, class B>
class bundle_property_map<Key, B, true> : public bundle_property_map<Key, B, false> {
    friend void put(bundle_property_map& pm, Key key, B value) { key.bundle() = value; }
};

template <class PM>
inline auto append_bundle_property_map(PM&& map) {
    return make_composite_property_map(
        std::forward<PM>(map), bundle_property_map<typename property_traits<PM>::value_type>());
}

}  // namespace ac::detail
