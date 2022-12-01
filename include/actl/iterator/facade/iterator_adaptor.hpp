// Iterator adaptor is designed to simplify creation of similar iterators.
// Adopted from boost:
// http://www.boost.org/doc/libs/1_65_1/libs/iterator/doc/iterator_adaptor.html
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/iterator/facade/iterator_facade.hpp>
#include <actl/iterator/traits/category.hpp>

namespace ac {

template <class Derived, class Iter, class Category = iter_category_t<Iter>>
class iterator_adaptor : public iterator_facade<Derived, Category>
{
    using base_t = iterator_facade<Derived, Category>;

public:
    // These are the default aliases that can be overwritten in the Derived
    // iterator when needed.
    using value_type = std::iter_value_t<Iter>;
    using difference_type = std::iter_difference_t<Iter>;

    explicit constexpr iterator_adaptor(Iter const& iter) noexcept(
        noexcept(Iter{iter}))
        : base_{iter}
    {}

    constexpr Iter const& base() const noexcept
    {
        return base_;
    }

    constexpr decltype(auto) operator*() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(*base())

    constexpr Derived& operator++()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(++base_ref(), base_t::derived())

    template <class C = Category>
    requires std::is_base_of_v<std::bidirectional_iterator_tag, C>
    constexpr Derived& operator--()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(--base_ref(), base_t::derived())

protected:
    constexpr Iter& base_ref() noexcept
    {
        return base_;
    }

private:
    friend struct ac::iterator_core_access;

    template <class Difference>
    constexpr auto advance(Difference n)
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(base_ref() += n, void())

    template <class Derived1, class Iter1, class Types1>
    constexpr bool equals(iterator_adaptor<Derived1, Iter1, Types1> const& rhs)
        const AC_DEDUCE_NOEXCEPT_AND_RETURN(base() == rhs.base())

    template <class Derived1, class Iter1, class Types1>
    constexpr auto distance_to(
        iterator_adaptor<Derived1, Iter1, Types1> const& rhs) const
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(rhs.base() - base())

    Iter base_;
};

} // namespace ac
