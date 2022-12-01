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
#include <actl/utility/operators.hpp>

namespace ac {

template <class Iter, class Category>
class iterator_facade : public operators::base<>
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

    constexpr Iter& operator--() AC_DEDUCE_NOEXCEPT_AND_RETURN(
        iterator_core_access::decrement(this->derived()), this->derived())

    constexpr Iter operator--(int) noexcept(
        noexcept(Iter{this->derived()}, --std::declval<Iter&>()))
    {
        Iter iter_copy = this->derived();
        --*this;
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
    constexpr decltype(auto) operator[](Difference n) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(*(this->derived() + n))

    template <class Difference>
    constexpr auto operator+=(Difference n)
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(
            iterator_core_access::advance(this->derived(), n), this->derived())

    template <class Difference>
    constexpr Iter& operator-=(Difference n)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(*this += -n)

    template <class Difference>
    constexpr Iter operator+(Difference n) const
        noexcept(noexcept(Iter{this->derived()}, std::declval<Iter&>() += n))
    {
        Iter iter_copy = this->derived();
        return iter_copy += n;
    }

    template <
        class Difference,
        enable_int_if<std::is_same_v<
            Iter&,
            decltype(std::declval<Iter&>() += -std::declval<Difference>())>> =
            0>
    constexpr Iter operator-(Difference n) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(*this + -n)
};

#define AC_ITERATOR_OPERATOR(op, expr)              \
    template <class Iter, class Category>           \
    constexpr auto operator op(                     \
        iterator_facade<Iter, Category> const& lhs, \
        iterator_facade<Iter, Category> const& rhs) \
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(expr)

AC_ITERATOR_OPERATOR(
    ==,
    iterator_core_access::equal(
        static_cast<Iter const&>(lhs), static_cast<Iter const&>(rhs)))

AC_ITERATOR_OPERATOR(
    -,
    iterator_core_access::distance_to(
        static_cast<Iter const&>(rhs), static_cast<Iter const&>(lhs)))

AC_ITERATOR_OPERATOR(<, lhs - rhs < 0)

#undef AC_ITERATOR_OPERATOR

template <class Iter, class Category, class Difference>
constexpr Iter operator+(
    Difference n, iterator_facade<Iter, Category> const& rhs)
    AC_DEDUCE_NOEXCEPT_AND_RETURN(rhs + n)

} // namespace ac
