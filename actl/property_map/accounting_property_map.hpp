/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/property_map/property_map.hpp>
#include <type_traits>
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
 * encountered before. Can be invertible with small overhead.
 */
template <class AssociativeContainer, bool Invertible = false,
          class Key   = const typename AssociativeContainer::key_type,
          class Value = typename AssociativeContainer::mapped_type>
class accounting_property_map
    : public container_property_map<Invertible, AssociativeContainer, Key, Value, Value, void>,
      public detail::vector_invert<Key, Value, Invertible> {
    static_assert(std::is_integral_v<Value>, "value type must be integral");

public:
    Value operator[](Key& key) {
        auto pair = this->data_.insert({key, static_cast<Value>(this->data_.size())});
        if (pair.second) this->push_back(&pair.first->first);
        return pair.first->second;
    }

    void clear() {
        container_property_map<Invertible, AssociativeContainer, Key, Value, Value, void>::clear();
        detail::vector_invert<Key, Value, Invertible>::clear();
    }
};

}  // namespace ac
