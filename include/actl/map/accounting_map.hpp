// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/map/traits.hpp>
#include <actl/range/traits/range_traits.hpp>
#include <actl/std/vector.hpp>
#include <actl/utility/compressed_pair.hpp>

namespace ac {

/// Property map into integer domain that assigns next non-negative integer to every key not
/// encountered before. Can be invertible with overhead of additional vector of pointers.
template <class AssociativeContainer, bool Invertible = false>
class accounting_map {
    using AC = AssociativeContainer;
    using K = typename AC::key_type;
    using Key = const K&;
    using V = typename AC::mapped_type;

public:
    static_assert(is_unique_range_v<AC> && is_pair_associative_range_v<AC>);
    static_assert(std::is_integral_v<V>, "value type must be integral");

    struct is_accounting_map;

    template <bool Const>
    using traits = map_traits_base<Key, const V&, V, !Const, false, Invertible, true, const AC&>;

    const V& get(Key key) {
        auto& ac = data_.first();
        auto pair = ac.insert({key, static_cast<V>(ac.size())});
        if constexpr (Invertible) {
            if (pair.second)
                data_.second().push_back(&pair.first->first);
        }
        return pair.first->second;
    }

    Key invert(const V& value) const {
        return *data_.second()[static_cast<size_t>(value)];
    }

    const AC& map_range() const {
        return data_.first();
    }

private:
    compressed_pair<AC, std::conditional_t<Invertible, std::vector<const K*>, none>> data_;
};

template <class AM>
struct map_traits<AM, std::void_t<typename AM::is_accounting_map>>
    : AM::template traits<std::is_const_v<AM>> {};

}  // namespace ac
