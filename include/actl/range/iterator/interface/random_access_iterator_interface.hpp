// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN.hpp>
#include <actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN.hpp>
#include <actl/meta/constant.hpp>
#include <actl/range/iterator/interface/bidirectional_iterator_interface.hpp>

namespace ac {

// https://en.cppreference.com/w/cpp/iterator/random_access_iterator
template<typename Iter>
class random_access_iterator_interface
    : public bidirectional_iterator_interface<Iter> {
public:
    using iterator_category = std::random_access_iterator_tag;

    template<typename Difference>
    constexpr auto operator[](Difference n) const
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(*(this->derived() + n))

    // This is a member function not a friend function
    // to allow overriding it in the derived iterator.
    constexpr Iter& operator++()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(this->derived() += constant<1>{})

    template<typename Difference>
    friend constexpr auto operator+(Iter iter, Difference n)
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(Iter{iter += n})

    template<typename Difference>
    // This constraint is needed to fix the gcc compilation error:
    // "satisfaction of atomic constraint depends on itself".
        requires(!std::input_or_output_iterator<Difference>)
    friend constexpr auto operator+(Difference n, Iter iter)
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(iter + n)

    // This is a member function not a friend function
    // to allow overriding it in the derived iterator.
    constexpr Iter& operator--()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(this->derived() += constant<-1>{})

    template<typename Difference>
    constexpr auto operator-=(Difference n)
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(this->derived() += -n)

    template<typename Difference>
    friend constexpr auto operator-(Iter iter, Difference n)
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(Iter{iter -= n})

    // random_access_iterator_interface is used as the parameter type here
    // to ensure that a custom user-defined operator== is a better match.
    constexpr bool operator==(random_access_iterator_interface const& rhs) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(this->derived() <=> rhs.derived() == 0)

    constexpr auto operator<=>(random_access_iterator_interface const& rhs
    ) const AC_DEDUCE_NOEXCEPT_AND_RETURN(this->derived() - rhs.derived() <=> 0)
};

} // namespace ac
