/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/iterator/iterator_facade.hpp>
#include <type_traits>

namespace ac {

template <class Int, bool Increment = true>
class integer_iterator
    : public iterator_facade<integer_iterator<Int, Increment>,
                             std::random_access_iterator_tag,
                             Int,
                             const Int&,
                             const Int*,
                             Int> {
public:
    explicit integer_iterator() = default;

    explicit integer_iterator(Int value) : value_{value} {}

private:
    const Int& dereference() const { return value_; }

    void increment() { Increment ? ++value_ : --value_; }

    void decrement() { Increment ? --value_ : ++value_; }

    void advance(Int n) { Increment ? value_ += n : value_ -= n; }

    bool equals(const integer_iterator& other) const { return value_ == other.value_; }

    Int distance_to(const integer_iterator& other) const { return other.value_ - value_; }

    Int value_;

    friend struct iterator_core_access;
};

template <class Int>
class integer_iterator_with_step
    : public iterator_facade<integer_iterator_with_step<Int>,
                             std::random_access_iterator_tag,
                             Int,
                             const Int&,
                             const Int*,
                             Int> {
public:
    explicit integer_iterator_with_step() : value_{}, step_{1} {}

    explicit integer_iterator_with_step(Int value, Int step) : value_{value}, step_{step} {}

private:
    const Int& dereference() const { return value_; }

    void increment() { value_ += step_; }

    void decrement() { value_ -= step_; }

    void advance(Int n) { value_ += n * step_; }

    bool equals(const integer_iterator_with_step& other) const { return value_ == other.value_; }

    // TODO: if this operation is called often then it's better to avoid division.
    Int distance_to(const integer_iterator_with_step& other) const {
        ACTL_ASSERT(other.step_ == step_);
        ACTL_ASSERT((other.value_ - value_) % step_ == 0);
        return (other.value_ - value_) / step_;
    }

    Int value_;
    Int step_;

    friend struct iterator_core_access;
};

}  // namespace ac
