// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/range/iterator/interface/random_access_iterator_interface.hpp>
#include <type_traits>

namespace ac {

template<typename Int, bool Increment = true>
class integer_iterator
    : public random_access_iterator_interface<
          integer_iterator<Int, Increment>> {
public:
    using value_type = Int;

    explicit constexpr integer_iterator() = default;

    explicit constexpr integer_iterator(Int value) : m_value{value} {}

    constexpr Int const& operator*() const noexcept {
        return m_value;
    }

    template<typename Difference>
    constexpr integer_iterator& operator+=(Difference n) noexcept {
        Increment ? m_value += n : m_value -= n;
        return *this;
    }

    friend constexpr Int operator-(
        integer_iterator const& lhs, integer_iterator const& rhs
    ) noexcept {
        Int diff = lhs.m_value - rhs.m_value;
        return Increment ? diff : -diff;
    }

private:
    Int m_value = 0;
};

template<typename Int>
class integer_iterator_with_step
    : public random_access_iterator_interface<integer_iterator_with_step<Int>> {
public:
    using value_type = Int;

    explicit constexpr integer_iterator_with_step() = default;

    explicit constexpr integer_iterator_with_step(Int value, Int step)
        : m_value{value}, m_step{step} {}

    constexpr Int const& operator*() const {
        return m_value;
    }

    template<typename Difference>
    constexpr integer_iterator_with_step& operator+=(Difference n) noexcept {
        m_value += n * m_step;
        return *this;
    }

    constexpr friend bool operator==(
        integer_iterator_with_step const& lhs,
        integer_iterator_with_step const& rhs
    ) noexcept {
        return lhs.m_value == rhs.m_value;
    }

    // TODO: if this operation is called often then it's better to avoid
    // division.
    friend constexpr Int operator-(
        integer_iterator_with_step const& lhs,
        integer_iterator_with_step const& rhs
    ) noexcept(AC_ASSERT_IS_NOEXCEPT()) {
        AC_ASSERT(lhs.m_step == rhs.m_step);
        AC_ASSERT((lhs.m_value - rhs.m_value) % lhs.m_step == 0);
        return (lhs.m_value - rhs.m_value) / lhs.m_step;
    }

private:
    Int m_value = 0;
    Int m_step = 1;
};

} // namespace ac
