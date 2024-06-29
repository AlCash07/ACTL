// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/traits.hpp>
#include <actl/memory/no_unique_address.hpp>
#include <actl/range/traits/associative_range.hpp>
#include <actl/range/traits/properties.hpp>
#include <actl/std/vector.hpp>

namespace ac {

/// Property map into integer domain that assigns next non-negative integer to
/// every key not encountered before. Can be invertible with overhead of
/// additional vector of pointers.
template <class AssociativeContainer, bool Invertible = false>
class accounting_map {
    using AC = AssociativeContainer;
    using K = typename AC::key_type;
    using Key = K const&;
    using V = typename AC::mapped_type;

public:
    static_assert(UniqueRange<AC> && PairAssociativeRange<AC>);
    static_assert(std::is_integral_v<V>, "value type must be integral");

    struct is_accounting_map;

    template <bool Const>
    using traits = map_traits_base<
        Key,
        V const&,
        V,
        !Const,
        false,
        Invertible,
        true,
        AC const&>;

    V const& get(Key key) {
        auto pair = range_.insert({key, static_cast<V>(range_.size())});
        if constexpr (Invertible) {
            if (pair.second)
                inverse_.push_back(&pair.first->first);
        }
        return pair.first->second;
    }

    Key invert(V const& value) const {
        return *inverse_[static_cast<size_t>(value)];
    }

    AC const& map_range() const {
        return range_;
    }

private:
    AC_NO_UNIQUE_ADDRESS AC range_;
    AC_NO_UNIQUE_ADDRESS
    std::conditional_t<Invertible, std::vector<K const*>, none> inverse_;
};

template <class AM>
    requires requires { typename AM::is_accounting_map; }
struct map_traits<AM> : AM::template traits<std::is_const_v<AM>> {};

} // namespace ac
