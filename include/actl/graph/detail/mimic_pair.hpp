/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/composite_map.hpp>
#include <actl/numeric/util/hash_access.hpp>
#include <actl/util/compressed_pair.hpp>
#include <actl/util/type_traits.hpp>

namespace ac::detail {

// Special compressed_pair that mimics either T1 or T2 (defined by index I) for the operations
// required by set and hash set.
template <class T1, class T2, index I>
class mimic_pair : public compressed_pair<T1, T2> {
    using base_t = compressed_pair<T1, T2>;

public:
    struct is_mimic_pair;
    using value_type = std::conditional_t<I == 1, T1, T2>;

    static_assert(I == 1 || I == 2);

    using base_t::base_t;

    constexpr decltype(auto) key() const {
        if constexpr (I == 1) {
            return this->first();
        } else {
            return this->second();
        }
    }

private:
    friend struct ac::hash_access;

    size_t hash() const { return hash_value(key()); }
};

template <class Key>
class second_map {
    using R = decltype(std::declval<Key>().second());

public:
    using traits =
        map_traits_base<Key, R, use_default, true, !std::is_const_v<std::remove_reference_t<R>>>;

    R get(Key key) { return key.second(); }

    void put(Key key, remove_cvref_t<R> value) { get(key) = value; }
};

template <class Map>
inline auto get_second(Map&& map) {
    return composite_map{std::forward<Map>(map), second_map<map_reference_t<Map>>{}};
}

}  // namespace ac::detail

namespace ac {

namespace op {

struct mimic_tag {
    struct has_nested;
};

template <class T>
struct category_impl<T, std::void_t<typename T::is_mimic_pair>> {
    using type = mimic_tag;
};

template <class T>
inline decltype(auto) get_key(const T& x) {
    if constexpr (std::is_same_v<category_t<T>, mimic_tag>) {
        return x.key();
    } else {
        return x;
    }
}

template <class Policy, class Op, class T, class U,
          enable_int_if<std::is_same_v<Op, Equal> || std::is_same_v<Op, Less>> = 0>
inline auto eval(mimic_tag, const Policy& policy, Op op, const T& lhs, const U& rhs) {
    return eval_dispatch(policy, op, get_key(lhs), get_key(rhs));
}

}  // namespace op

template <class K>
struct map_traits<detail::second_map<K>> : detail::second_map<K>::traits {};

}  // namespace ac
