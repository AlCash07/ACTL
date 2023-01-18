// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/iterator/interface/bidirectional_iterator_interface.hpp>
#include <actl/meta/constant.hpp>

namespace ac {

// https://en.cppreference.com/w/cpp/iterator/random_access_iterator
template <class Iter>
class random_access_iterator_interface
    : public bidirectional_iterator_interface<Iter>
{
public:
    using iterator_category = std::random_access_iterator_tag;

    template <class Difference>
    constexpr auto operator[](Difference n) const
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(*(this->derived() + n))

    // This is a method to allow overwriting it in the derived iterator.
    constexpr Iter& operator++()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(this->derived() += constant<1>{})

    template <class Difference>
    friend constexpr Iter operator+(Iter iter, Difference n)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(Iter{iter += n})

    template <class Difference>
    // This constraint is needed to disambiguate this overload from
    // `iter + n` when Difference == Iter.
    // However, only gcc seems to complain currently.
        requires(!std::same_as<Difference, Iter>)
    friend constexpr Iter operator+(Difference n, Iter iter)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(iter + n)

    // This is a method to allow overwriting it in the derived iterator.
    constexpr Iter& operator--()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(this->derived() += constant<-1>{})

    template <class Difference>
    friend constexpr Iter& operator-=(Iter& iter, Difference n)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(iter += -n)

    template <class Difference>
    friend constexpr Iter operator-(Iter iter, Difference n)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(Iter{iter -= n})

    friend bool operator==(
        random_access_iterator_interface const& lhs,
        random_access_iterator_interface const& rhs)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(lhs <=> rhs == 0)

    friend auto operator<=>(
        random_access_iterator_interface const& lhs,
        random_access_iterator_interface const& rhs)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(lhs.derived() - rhs.derived() <=> 0)
};

} // namespace ac
