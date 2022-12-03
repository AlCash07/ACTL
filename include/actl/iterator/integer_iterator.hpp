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

template <class Int, bool Increment = true>
class integer_iterator
    : public iterator_facade<
          integer_iterator<Int, Increment>,
          std::random_access_iterator_tag>
{
public:
    using value_type = Int;

    explicit constexpr integer_iterator() = default;

    explicit constexpr integer_iterator(Int value) : value_{value} {}

    constexpr Int const& operator*() const noexcept
    {
        return value_;
    }

    constexpr integer_iterator& operator++() noexcept
    {
        Increment ? ++value_ : --value_;
        return *this;
    }

    constexpr integer_iterator& operator--() noexcept
    {
        Increment ? --value_ : ++value_;
        return *this;
    }

    constexpr integer_iterator& operator+=(Int n)
    {
        Increment ? value_ += n : value_ -= n;
        return *this;
    }

    constexpr friend bool operator==(
        integer_iterator const& lhs, integer_iterator const& rhs) noexcept
    {
        return lhs.value_ == rhs.value_;
    }

    friend constexpr Int operator-(
        integer_iterator const& lhs, integer_iterator const& rhs) noexcept
    {
        Int diff = lhs.value_ - rhs.value_;
        return Increment ? diff : -diff;
    }

private:
    Int value_ = 0;
};

template <class Int>
class integer_iterator_with_step
    : public iterator_facade<
          integer_iterator_with_step<Int>,
          std::random_access_iterator_tag>
{
public:
    using value_type = Int;

    explicit constexpr integer_iterator_with_step() = default;

    explicit constexpr integer_iterator_with_step(Int value, Int step)
        : value_{value}, step_{step}
    {}

    constexpr Int const& operator*() const
    {
        return value_;
    }

    constexpr integer_iterator_with_step& operator++() noexcept
    {
        value_ += step_;
        return *this;
    }

    constexpr integer_iterator_with_step& operator--() noexcept
    {
        value_ -= step_;
        return *this;
    }

    constexpr integer_iterator_with_step& operator+=(Int n) noexcept
    {
        value_ += n * step_;
        return *this;
    }

    constexpr friend bool operator==(
        integer_iterator_with_step const& lhs,
        integer_iterator_with_step const& rhs) noexcept
    {
        return lhs.value_ == rhs.value_;
    }

    // TODO: if this operation is called often then it's better to avoid
    // division.
    friend constexpr Int operator-(
        integer_iterator_with_step const& lhs,
        integer_iterator_with_step const& rhs) noexcept(AC_ASSERT_IS_NOEXCEPT())
    {
        AC_ASSERT(lhs.step_ == rhs.step_);
        AC_ASSERT((lhs.value_ - rhs.value_) % lhs.step_ == 0);
        return (lhs.value_ - rhs.value_) / lhs.step_;
    }

private:
    Int value_ = 0;
    Int step_ = 1;
};

} // namespace ac
