/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>
#include <vector>

namespace ac {

namespace detail {

template <class Key, class Value, bool Invertible = false>
class vector_invert {
protected:
    void push_back(Key*) {}
    void clear() {}
};

template <class Key, class Value>
class vector_invert<Key, Value, true> {
public:
    Key invert(Value value) const { return *keys_[value]; }

protected:
    void push_back(Key* ptr) { keys_.push_back(ptr); }
    void clear() { keys_.clear(); }

private:
    std::vector<Key*> keys_;
};

}  // namespace detail

/**
 * Property map into integer domain that assigns next non-negative integer to every key not
 * encountered before. Can be invertible with overhead of additional vector of pointers.
 */
template <class AssociativeContainer, bool Invertible = false,
          class Key   = typename AssociativeContainer::key_type,
          class Value = typename AssociativeContainer::mapped_type>
class accounting_property_map
    : public container_property_map<AssociativeContainer, Key, Value, Value, Invertible, false>,
      public detail::vector_invert<Key, Value, Invertible> {
public:
    static_assert(std::is_integral_v<Value>, "value type must be integral");

    friend Value get(accounting_property_map& pm, Key key) {
        auto pair = pm.data_.insert({key, static_cast<Value>(pm.data_.size())});
        if (pair.second) pm.push_back(&pair.first->first);
        return pair.first->second;
    }

    void clear() {
        this->data_.clear();
        detail::vector_invert<Key, Value, Invertible>::clear();
    }
};

}  // namespace ac
