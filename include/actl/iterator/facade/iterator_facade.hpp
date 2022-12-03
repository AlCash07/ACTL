// Iterator facade is designed to produce complete iterator interface from a set
// of core functions. Adopted from boost (with simplifications):
// http://www.boost.org/doc/libs/1_65_1/libs/iterator/doc/iterator_facade.html
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/deduce_noexcept.hpp>
#include <actl/iterator/facade/arrow_operator.hpp>
#include <actl/meta/constant.hpp>

namespace ac {

template <class Iter, class Category>
class iterator_facade
{
protected:
    constexpr Iter& derived() noexcept
    {
        return static_cast<Iter&>(*this);
    }

    constexpr Iter const& derived() const noexcept
    {
        return static_cast<Iter const&>(*this);
    }

public:
    using iterator_category = Category;

    // This is a free function so that it's not hidden in the derived iterator
    // which defines operator++().
    friend constexpr Iter operator++(Iter& iter, int) noexcept(
        noexcept(Iter{iter}, ++iter))
    {
        Iter iter_copy = iter;
        ++iter;
        return iter_copy;
    }
};

template <class Iter>
class iterator_facade<Iter, std::input_iterator_tag>
    : public iterator_facade<Iter, std::output_iterator_tag>
{
protected:
    using base_t = iterator_facade<Iter, std::output_iterator_tag>;

public:
    using iterator_category = std::input_iterator_tag;

    constexpr auto operator->() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(detail::arrow_operator(*this->derived()))
};

template <class Iter>
class iterator_facade<Iter, std::forward_iterator_tag>
    : public iterator_facade<Iter, std::input_iterator_tag>
{
public:
    using iterator_category = std::forward_iterator_tag;
};

template <class Iter>
class iterator_facade<Iter, std::bidirectional_iterator_tag>
    : public iterator_facade<Iter, std::forward_iterator_tag>
{
public:
    using iterator_category = std::bidirectional_iterator_tag;

    // This is a free function so that it's not hidden in the derived iterator
    // which defines operator--().
    friend constexpr Iter operator--(Iter& iter, int) noexcept(
        noexcept(Iter{iter}, --iter))
    {
        Iter iter_copy = iter;
        --iter;
        return iter_copy;
    }
};

template <class Iter>
class iterator_facade<Iter, std::random_access_iterator_tag>
    : public iterator_facade<Iter, std::bidirectional_iterator_tag>
{
protected:
    using base_t = iterator_facade<Iter, std::bidirectional_iterator_tag>;

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
        iterator_facade const& lhs, iterator_facade const& rhs)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(lhs <=> rhs == 0)

    friend auto operator<=>(
        iterator_facade const& lhs, iterator_facade const& rhs)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(lhs.derived() - rhs.derived() <=> 0)
};

} // namespace ac
