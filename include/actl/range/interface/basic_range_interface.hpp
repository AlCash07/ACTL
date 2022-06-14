// Range interface is designed to produce complete range interface from a class
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
#include <actl/iterator/traits/is_const_iterator.hpp>
#include <actl/range/interface/range_types.hpp>
#include <iterator>

namespace ac {

template <class Derived, class Types>
class basic_range_interface : public detail::range_types<Types>
{
public:
    constexpr bool empty() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(derived().begin() == derived().end())

    explicit constexpr operator bool() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(!empty())

    constexpr decltype(auto) front() const
        noexcept(ACTL_ASSERT_IS_NOEXCEPT() && noexcept(*derived().begin()))
    {
        ACTL_ASSERT(!empty());
        return *derived().begin();
    }

protected:
    constexpr Derived const& derived() const noexcept
    {
        return static_cast<Derived const&>(*this);
    }
};

template <class Range, class Types, class Category>
struct range_interface_selector
{
    using type = basic_range_interface<Range, Types>;
};

template <
    class Range,
    class Types,
    class Category = iter_category_t<typename Types::iterator>>
using range_interface_selector_t =
    typename range_interface_selector<Range, Types, Category>::type;

} // namespace ac
