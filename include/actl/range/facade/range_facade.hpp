// Range facade is designed to produce complete range interface from a class
// with just two methods begin() and end() that return iterators.
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/functional/deduce_noexcept.hpp>
#include <actl/iterator/traits.hpp>
#include <actl/range/facade/range_types.hpp>
#include <iterator>

namespace ac {

namespace detail {

#define ENABLE_IF_HAS_CONST \
    template <class U = T, class = typename U::const_iterator>

template <class D, class T, class C>
class rng_facade : public T
{
public:
    ENABLE_IF_HAS_CONST constexpr auto cbegin() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(derived().begin())

    ENABLE_IF_HAS_CONST constexpr auto cend() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(derived().end())

    constexpr bool empty() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(derived().begin() == derived().end())

    ENABLE_IF_HAS_CONST constexpr bool empty()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(derived().begin() == derived().end())

    explicit constexpr operator bool() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(!empty())

    ENABLE_IF_HAS_CONST explicit constexpr operator bool()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(!empty())

    constexpr decltype(auto) front() const
        noexcept(ACTL_ASSERT_IS_NOEXCEPT() && noexcept(*derived().begin()))
    {
        ACTL_ASSERT(!empty());
        return *derived().begin();
    }

    ENABLE_IF_HAS_CONST constexpr decltype(auto) front() noexcept(
        ACTL_ASSERT_IS_NOEXCEPT() && noexcept(*derived().begin()))
    {
        ACTL_ASSERT(!empty());
        return *derived().begin();
    }

protected:
    constexpr const D& derived() const noexcept
    {
        return static_cast<const D&>(*this);
    }

    constexpr D& derived() noexcept
    {
        return static_cast<D&>(*this);
    }
};

template <class T, class = void>
struct crev_iter
{
    using type = typename T::reverse_iterator;
};

template <class T>
struct crev_iter<T, std::void_t<typename T::const_reverse_iterator>>
{
    using type = typename T::const_reverse_iterator;
};

template <class T>
using crev_iter_t = typename crev_iter<T>::type;

template <class D, class T>
class rng_facade<D, T, std::bidirectional_iterator_tag>
    : public rng_facade<D, T, std::forward_iterator_tag>
{
protected:
    using rng_facade<D, T, std::forward_iterator_tag>::derived;

public:
    constexpr auto rbegin() const
    {
        return crev_iter_t<T>{derived().end()};
    }

    ENABLE_IF_HAS_CONST constexpr auto rbegin()
    {
        return typename T::reverse_iterator{derived().end()};
    }

    ENABLE_IF_HAS_CONST constexpr auto crbegin() const
    {
        return rbegin();
    }

    constexpr auto rend() const
    {
        return crev_iter_t<T>{derived().begin()};
    }

    ENABLE_IF_HAS_CONST constexpr auto rend()
    {
        return typename T::reverse_iterator{derived().begin()};
    }

    ENABLE_IF_HAS_CONST constexpr auto crend() const
    {
        return rend();
    }

    constexpr decltype(auto) back() const
        noexcept(ACTL_ASSERT_IS_NOEXCEPT() && noexcept(*--derived().end()))
    {
        ACTL_ASSERT(!this->empty());
        auto last = derived().end();
        return *--last;
    }

    ENABLE_IF_HAS_CONST constexpr decltype(auto) back() noexcept(
        ACTL_ASSERT_IS_NOEXCEPT() && noexcept(*--derived().end()))
    {
        ACTL_ASSERT(!this->empty());
        auto last = derived().end();
        return *--last;
    }
};

template <class D, class T>
class rng_facade<D, T, std::random_access_iterator_tag>
    : public rng_facade<D, T, std::bidirectional_iterator_tag>
{
    using base_t = rng_facade<D, T, std::bidirectional_iterator_tag>;
    using base_t::derived;

public:
    using typename base_t::difference_type;
    using typename base_t::size_type;

    constexpr decltype(auto) operator[](size_type n) const
        noexcept(ACTL_ASSERT_IS_NOEXCEPT() && noexcept(
            derived().begin()[static_cast<difference_type>(n)]))
    {
        ACTL_ASSERT(0 <= n && n < size());
        return derived().begin()[static_cast<difference_type>(n)];
    }

    ENABLE_IF_HAS_CONST constexpr decltype(auto)
    operator[](size_type n) noexcept(ACTL_ASSERT_IS_NOEXCEPT() && noexcept(
        derived().begin()[static_cast<difference_type>(n)]))
    {
        ACTL_ASSERT(0 <= n && n < size());
        return derived().begin()[static_cast<difference_type>(n)];
    }

    constexpr auto size() const AC_DEDUCE_NOEXCEPT_AND_RETURN(
        static_cast<size_type>(derived().end() - derived().begin()))

    ENABLE_IF_HAS_CONST constexpr auto size() AC_DEDUCE_NOEXCEPT_AND_RETURN(
        static_cast<size_type>(derived().end() - derived().begin()))
};

#undef ENABLE_IF_HAS_CONST

} // namespace detail

template <class Range, class Types>
using range_facade = detail::rng_facade<
    Range,
    detail::range_types<Types>,
    iterator_category_t<typename Types::iterator>>;

} // namespace ac
