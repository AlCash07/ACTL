// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/iterator/facade/iterator_facade.hpp>
#include <type_traits>

namespace ac {

template <class Int>
struct integer_iterator_types
{
    using iterator_category = std::random_access_iterator_tag;
    using value_type = Int;
    using reference = Int const&;
    using difference_type = Int;
};

template <class Int, bool Increment = true>
class integer_iterator
    : public iterator_facade<
          integer_iterator<Int, Increment>,
          integer_iterator_types<Int>>
{
public:
    explicit constexpr integer_iterator() = default;

    explicit constexpr integer_iterator(Int value) : value_{value} {}

private:
    friend struct ac::iterator_core_access;

    constexpr Int const& dereference() const
    {
        return value_;
    }

    constexpr void increment()
    {
        Increment ? ++value_ : --value_;
    }

    constexpr void decrement()
    {
        Increment ? --value_ : ++value_;
    }

    constexpr void advance(Int n)
    {
        Increment ? value_ += n : value_ -= n;
    }

    constexpr bool equals(integer_iterator const& rhs) const
    {
        return value_ == rhs.value_;
    }

    constexpr Int distance_to(integer_iterator const& rhs) const
    {
        Int diff = rhs.value_ - value_;
        return Increment ? diff : -diff;
    }

    Int value_ = 0;
};

template <class Int>
class integer_iterator_with_step
    : public iterator_facade<
          integer_iterator_with_step<Int>,
          integer_iterator_types<Int>>
{
public:
    explicit constexpr integer_iterator_with_step() = default;

    explicit constexpr integer_iterator_with_step(Int value, Int step)
        : value_{value}, step_{step}
    {}

private:
    friend struct ac::iterator_core_access;

    constexpr Int const& dereference() const
    {
        return value_;
    }

    constexpr void increment()
    {
        value_ += step_;
    }

    constexpr void decrement()
    {
        value_ -= step_;
    }

    constexpr void advance(Int n)
    {
        value_ += n * step_;
    }

    constexpr bool equals(integer_iterator_with_step const& rhs) const
    {
        return value_ == rhs.value_;
    }

    // TODO: if this operation is called often then it's better to avoid
    // division.
    constexpr Int distance_to(integer_iterator_with_step const& rhs) const
    {
        ACTL_ASSERT(rhs.step_ == step_);
        ACTL_ASSERT((rhs.value_ - value_) % step_ == 0);
        return (rhs.value_ - value_) / step_;
    }

    Int value_ = 0;
    Int step_ = 1;
};

} // namespace ac
