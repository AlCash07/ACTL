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

#include <actl/iterator/facade/iterator_core_access.hpp>
#include <actl/iterator/facade/iterator_types.hpp>
#include <actl/iterator/facade/operator_arrow_dispatch.hpp>
#include <actl/utility/operators.hpp>

namespace ac {

namespace detail {

template <class Iter, class T, class C>
class iter_facade : public operators::base<>
{
public:
    using iterator_category = typename T::iterator_category;
    using value_type = std::remove_cv_t<typename T::value_type>;
    using reference = deduce_t<typename T::reference, value_type&>;
    using pointer = typename operator_arrow_dispatch<reference>::type;
    using difference_type =
        deduce_t<typename T::difference_type, std::ptrdiff_t>;

    constexpr reference operator*() const AC_DEDUCE_NOEXCEPT_AND_RETURN(
        iterator_core_access::dereference(derived()))

    constexpr Iter& operator++() AC_DEDUCE_NOEXCEPT_AND_RETURN(
        iterator_core_access::increment(derived()), derived())

    constexpr Iter operator++(int) noexcept(
        noexcept(Iter{derived()}, ++std::declval<Iter&>()))
    {
        Iter copy = derived();
        ++*this;
        return copy;
    }

protected:
    constexpr Iter& derived() noexcept
    {
        return static_cast<Iter&>(*this);
    }

    constexpr const Iter& derived() const noexcept
    {
        return static_cast<const Iter&>(*this);
    }
};

template <class Iter, class T>
class iter_facade<Iter, T, std::input_iterator_tag>
    : public iter_facade<Iter, T, std::output_iterator_tag>
{
protected:
    using base_t = iter_facade<Iter, T, std::output_iterator_tag>;

public:
    using typename base_t::reference;

    constexpr typename base_t::pointer operator->() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(
            operator_arrow_dispatch<reference>::apply(*this->derived()))
};

template <class Iter, class T>
class iter_facade<Iter, T, std::forward_iterator_tag>
    : public iter_facade<Iter, T, std::input_iterator_tag>
{};

template <class Iter, class T>
class iter_facade<Iter, T, std::bidirectional_iterator_tag>
    : public iter_facade<Iter, T, std::forward_iterator_tag>
{
public:
    constexpr Iter& operator--() AC_DEDUCE_NOEXCEPT_AND_RETURN(
        iterator_core_access::decrement(this->derived()), this->derived())

    constexpr Iter operator--(int) noexcept(
        noexcept(Iter{this->derived()}, --std::declval<Iter&>()))
    {
        Iter copy = this->derived();
        --*this;
        return copy;
    }
};

template <class Iter, class T>
class iter_facade<Iter, T, std::random_access_iterator_tag>
    : public iter_facade<Iter, T, std::bidirectional_iterator_tag>
{
protected:
    using base_t = iter_facade<Iter, T, std::bidirectional_iterator_tag>;

public:
    using typename base_t::difference_type;

    constexpr typename base_t::reference operator[](difference_type n) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(*(this->derived() + n))

    constexpr Iter& operator+=(difference_type n) AC_DEDUCE_NOEXCEPT_AND_RETURN(
        iterator_core_access::advance(this->derived(), n), this->derived())

    constexpr Iter& operator-=(difference_type n)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(*this += -n)

    constexpr Iter operator+(difference_type n) const
        noexcept(noexcept(Iter{this->derived()}, std::declval<Iter&>() += n))
    {
        Iter copy = this->derived();
        return copy += n;
    }

    constexpr Iter operator-(difference_type n) const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(*this + -n)
};

} // namespace detail

template <class Derived, class Types>
class iterator_facade
    : public detail::
          iter_facade<Derived, Types, typename Types::iterator_category>
{};

#define ACTL_ITERATOR_OPERATOR(type, op, expr) \
    template <class Iter, class T>             \
    constexpr type operator op(                \
        const iterator_facade<Iter, T>& lhs,   \
        const iterator_facade<Iter, T>& rhs)   \
        AC_DEDUCE_NOEXCEPT_AND_RETURN(expr)

ACTL_ITERATOR_OPERATOR(
    bool,
    ==,
    iterator_core_access::equal(
        static_cast<const Iter&>(lhs), static_cast<const Iter&>(rhs)))

ACTL_ITERATOR_OPERATOR(
    auto,
    -,
    iterator_core_access::distance_to(
        static_cast<const Iter&>(rhs), static_cast<const Iter&>(lhs)))

ACTL_ITERATOR_OPERATOR(bool, <, lhs - rhs < 0)

#undef ACTL_ITERATOR_OPERATOR

template <class Iter, class T>
constexpr Iter operator+(
    typename T::difference_type n, const iterator_facade<Iter, T>& rhs)
    AC_DEDUCE_NOEXCEPT_AND_RETURN(rhs + n)

} // namespace ac
