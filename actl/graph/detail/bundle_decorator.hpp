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
// MimicT indicates whether operation required by set and hash set are inherited from T or B.
template <class T, class B, bool MimicT = true, bool = std::is_empty_v<B> && !std::is_final_v<B>>
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

template <class T, class B, bool MimicT>
class bundle_decorator<T, B, MimicT, false> : public T {
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

template <class T, class B>
inline bool operator < (const bundle_decorator<T, B, false>& lhs,
                        const bundle_decorator<T, B, false>& rhs) {
    return lhs.bundle() < rhs.bundle();
}

template <class T, class B>
inline bool operator == (const bundle_decorator<T, B, false>& lhs,
                         const bundle_decorator<T, B, false>& rhs) {
    return lhs.bundle() == rhs.bundle();
}

template <class Key>
struct bundle_pm_traits {
    using T = std::remove_reference_t<Key>;
    using B = typename T::bundle_type;

    static constexpr bool writable = !is_const_v<T>;

    using base = property_map<Key, B, add_const_if_t<!writable, B&>, false, false, writable>;
};

template <class Key, bool W = bundle_pm_traits<Key>::writable>
class bundle_property_map : public bundle_pm_traits<Key>::base {
    friend typename bundle_pm_traits<Key>::base::reference get(const bundle_property_map& pm,
                                                               Key key) {
        return key.bundle();
    }
};

template <class Key>
class bundle_property_map<Key, true> : public bundle_property_map<Key, false> {
    friend void put(const bundle_property_map& pm, Key key,
                    typename bundle_pm_traits<Key>::B value) {
        key.bundle() = value;
    }
};

template <class PM>
inline auto append_bundle_property_map(PM&& map) {
    return make_composite_property_map(
        std::forward<PM>(map), bundle_property_map<typename property_traits<PM>::reference>());
}

}  // namespace ac::detail

namespace std {

template <class T, class B>
struct hash<ac::detail::bundle_decorator<T, B, true>> : hash<T> {};

template <class T, class B>
struct hash<ac::detail::bundle_decorator<T, B, false>> {
    auto operator()(const ac::detail::bundle_decorator<T, B, false>& arg) const {
        return std::hash<B>{}(arg.bundle());
    }
};

}  // namespace std
