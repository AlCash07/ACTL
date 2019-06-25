/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/map/property_map.hpp>
#include <actl/std/vector.hpp>

namespace ac {

namespace detail {

template <class Key, class Value, bool Invertible = false>
class vector_invert {
protected:
    void push_back(const Key*) const {}
    void clear_vector() const {}
};

template <class Key, class Value>
class vector_invert<Key, Value, true> {
public:
    Key invert(Value value) const {
        return *keys_[static_cast<typename std::vector<const Key*>::size_type>(value)];
    }

protected:
    void push_back(const Key* ptr) const { keys_.push_back(ptr); }
    void clear_vector() const { keys_.clear(); }

private:
    mutable std::vector<const Key*> keys_;
};

}  // namespace detail

/**
 * Property map into integer domain that assigns next non-negative integer to every key not
 * encountered before. Can be invertible with overhead of additional vector of pointers.
 */
template <class AssociativeContainer, bool Invertible = false,
          class Key = typename AssociativeContainer::key_type,
          class Value = typename AssociativeContainer::mapped_type>
class accounting_map : public property_map<Key, Value, Value, Invertible, true>,
                       public detail::vector_invert<Key, Value, Invertible> {
public:
    using iterator = typename AssociativeContainer::const_iterator;

    static_assert(std::is_integral_v<Value>, "value type must be integral");

    friend Value get(const accounting_map& map, Key key) {
        auto pair = map.data_.insert({key, static_cast<Value>(map.data_.size())});
        if (pair.second) map.push_back(&pair.first->first);
        return pair.first->second;
    }

    iterator begin() const { return data_.begin(); }
    iterator end() const { return data_.end(); }

    void clear() {
        this->data_.clear();
        this->clear_vector();
    }

private:
    mutable AssociativeContainer data_;
};

}  // namespace ac