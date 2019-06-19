/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/numeric/hash.hpp>
#include <actl/property_map/composite_property_map.hpp>
#include <actl/traits/type_traits.hpp>
#include <actl/util/compressed_pair.hpp>

namespace ac::detail {

// Special compressed_pair that mimics either T1 or T2 (defined by index I) for the operations
// required by set and hash set.
template <class T1, class T2, int I>
class mimic_pair : public compressed_pair<T1, T2> {
    using base_t = compressed_pair<T1, T2>;

public:
    static_assert(I == 1 || I == 2);

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

    friend size_t hash_value(const mimic_pair& x) {
        using ac::hash_value;
        return hash_value(x.key());
    }
};

template <class T1, class T2, int I>
using key_type_t = std::conditional_t<I == 1, T1, T2>;

template <class T1, class T2, int I>
inline bool operator < (const mimic_pair<T1, T2, I>& lhs, const key_type_t<T1, T2, I>& rhs) {
    return lhs.key() < rhs;
}

template <class T1, class T2, int I>
inline bool operator < (const key_type_t<T1, T2, I>& lhs, const mimic_pair<T1, T2, I>& rhs) {
    return lhs < rhs.key();
}

template <class T1, class T2, int I>
inline bool operator == (const mimic_pair<T1, T2, I>& lhs, const key_type_t<T1, T2, I>& rhs) {
    return lhs.key() == rhs;
}

template <class T1, class T2, int I>
inline bool operator == (const key_type_t<T1, T2, I>& lhs, const mimic_pair<T1, T2, I>& rhs) {
    return lhs == rhs.key();
}

template <class Key>
struct second_pm_traits {
    using T = std::remove_reference_t<Key>;
    using B = typename T::second_type;

    static constexpr bool writable = !std::is_const_v<T>;

    using base = property_map<Key, B, add_const_if_t<!writable, B&>, false, false, writable>;
};

template <class Key>
class second_property_map : public put_helper<second_property_map<Key>>,
                            public second_pm_traits<Key>::base {
    friend typename second_pm_traits<Key>::base::reference get(second_property_map, Key key) {
        return key.second();
    }
};

template <class PM>
inline auto get_second(PM&& map) {
    return composite_property_map{std::forward<PM>(map),
                                  second_property_map<typename property_traits<PM>::reference>{}};
}

}  // namespace ac::detail
