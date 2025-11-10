// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/memory/AC_NO_UNIQUE_ADDRESS.hpp>
#include <actl/meta/constant.hpp>
#include <actl/range/iterator/interface/random_access_iterator_interface.hpp>

namespace ac {

/// Iterator over an integer arithmetic sequence.
/// @tparam Step difference between successive members in the sequence.
/// It's 1 by default, which results in consecutive increasing numbers.
template<typename Int, typename Step = constant<1>>
class integer_iterator
    : public random_access_iterator_interface<integer_iterator<Int, Step>> {
    Int m_value;
    AC_NO_UNIQUE_ADDRESS Step m_step;

public:
    using value_type = Int;

    explicit constexpr integer_iterator()
        : m_value{0}, m_step{[] {
            if constexpr (is_constant_v<Step>)
                return Step{};
            else
                return 1; // to avoid division by 0
        }()} {}

    explicit constexpr integer_iterator(Int value)
        requires is_constant_v<Step>
        : m_value{value} {}

    explicit constexpr integer_iterator(Int value, Step step)
        : m_value{value}, m_step{step} {}

    constexpr Int const& operator*() const {
        return m_value;
    }

    constexpr friend bool operator==(
        integer_iterator const& l, integer_iterator const& r
    ) AC_DEDUCE_NOEXCEPT_AND_RETURN( //
        l.m_value == r.m_value
    )

    template<typename Difference>
    constexpr auto operator+=(Difference offset)
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN( //
            m_value += offset * m_step,
            *this
        )

    // Note: the return type could be different from Int.
    friend constexpr auto
    operator-(integer_iterator const& l, integer_iterator const& r) noexcept(
        AC_ASSERT_IS_NOEXCEPT() && noexcept((l.m_value - r.m_value) / l.m_step)
    ) {
        AC_ASSERT(l.m_step == r.m_step);
        AC_ASSERT((l.m_value - r.m_value) % l.m_step == 0);
        // TODO: if this operation is called often
        // then it's better to avoid division.
        return (l.m_value - r.m_value) / l.m_step;
    }
};

} // namespace ac
