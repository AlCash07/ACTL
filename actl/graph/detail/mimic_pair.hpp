/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/composite_property_map.hpp>
#include <actl/traits/type_traits.hpp>
#include <actl/type/compressed_pair.hpp>

namespace ac::detail {

// Special compressed_pair that mimics either T1 or T2 (defined by index I) for the operations
// required by set and hash set.
template <class T1, class T2, int I>
class mimic_pair : public compressed_pair<T1, T2> {
    using base_t = compressed_pair<T1, T2>;

public:
    using base_t::base_t;

    constexpr auto key() const {
        if constexpr (I == 1) {
            return this->first();
        } else {
            return this->second();
        }
    }

    bool operator < (const mimic_pair& rhs) const { return key() < rhs.key(); }
    bool operator == (const mimic_pair& rhs) const { return key() == rhs.key(); }
};

template <class Key>
struct second_pm_traits {
    using T = std::remove_reference_t<Key>;
    using B = typename T::second_type;

    static constexpr bool writable = !is_const_v<T>;

    using base = property_map<Key, B, add_const_if_t<!writable, B&>, false, false, writable>;
};

template <class Key, bool W = second_pm_traits<Key>::writable>
class second_property_map : public second_pm_traits<Key>::base {
    friend typename second_pm_traits<Key>::base::reference get(const second_property_map&,
                                                               Key key) {
        return key.second();
    }
};

template <class Key>
class second_property_map<Key, true> : public second_property_map<Key, false> {
    friend void put(const second_property_map&, Key key, typename second_pm_traits<Key>::B value) {
        key.second() = value;
    }
};

template <class PM>
inline auto get_second(PM&& map) {
    return make_composite_property_map(
        std::forward<PM>(map), second_property_map<typename property_traits<PM>::reference>());
}

}  // namespace ac::detail

namespace std {

template <class T1, class T2, int I>
struct hash<ac::detail::mimic_pair<T1, T2, I>> {
    auto operator()(const ac::detail::mimic_pair<T1, T2, I>& arg) const {
        const auto& key = arg.key();
        return hash<ac::remove_cvref_t<decltype(key)>>{}(key);
    }
};

}  // namespace std