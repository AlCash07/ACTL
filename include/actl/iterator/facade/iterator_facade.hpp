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

#include <actl/iterator/facade/arrow_operator.hpp>
#include <actl/iterator/facade/iterator_core_access.hpp>

namespace ac {

template <class Iter, class Category>
class iterator_facade
{
public:
    using iterator_category = Category;

    friend constexpr Iter operator++(Iter& iter, int) noexcept(
        noexcept(Iter{iter}, ++iter))
    {
        Iter iter_copy = iter;
        ++iter;
        return iter_copy;
    }

protected:
    constexpr Iter& derived() noexcept
    {
        return static_cast<Iter&>(*this);
    }

    constexpr Iter const& derived() const noexcept
    {
        return static_cast<Iter const&>(*this);
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

    template <class Difference>
    friend constexpr Iter operator+(Iter iter, Difference n)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(Iter{iter += n})

    template <class Difference>
    friend constexpr Iter operator+(Difference n, Iter iter)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(iter + n)

    template <class Difference>
    friend constexpr Iter& operator-=(Iter& iter, Difference n)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(iter += -n)

    template <class Difference>
    friend constexpr Iter operator-(Iter iter, Difference n)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(Iter{iter -= n})

    friend auto operator<=>(Iter const& lhs, Iter const& rhs)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(lhs - rhs <=> 0)
};

#define AC_ITERATOR_OPERATOR(op, expr)              \
    template <class Iter, class Category>           \
    constexpr auto operator op(                     \
        iterator_facade<Iter, Category> const& lhs, \
        iterator_facade<Iter, Category> const& rhs) \
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(expr)

AC_ITERATOR_OPERATOR(
    -,
    iterator_core_access::distance_to(
        static_cast<Iter const&>(rhs), static_cast<Iter const&>(lhs)))

#undef AC_ITERATOR_OPERATOR

} // namespace ac
