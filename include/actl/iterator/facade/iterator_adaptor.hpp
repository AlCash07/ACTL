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
#include <actl/iterator/facade/iterator_types.hpp>
#include <actl/iterator/traits/category.hpp>

namespace ac {

template <class Derived, class Iter, class Types = void>
class iterator_adaptor
    : public iterator_facade<
          Derived,
          deduce_t<typename Types::iterator_category, iter_category_t<Iter>>>
{
public:
    using value_type =
        deduce_t<typename Types::value_type, std::iter_value_t<Iter>>;
    using reference =
        deduce_t<typename Types::reference, std::iter_reference_t<Iter>>;
    using difference_type =
        deduce_t<typename Types::difference_type, std::iter_difference_t<Iter>>;

    explicit constexpr iterator_adaptor(Iter const& iter) noexcept(
        noexcept(Iter{iter}))
        : base_{iter}
    {}

    constexpr Iter const& base() const noexcept
    {
        return base_;
    }

protected:
    constexpr Iter& base_ref() noexcept
    {
        return base_;
    }

private:
    friend struct ac::iterator_core_access;

    constexpr decltype(auto) dereference() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(*base())

    constexpr void increment()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(++base_ref(), void())

    constexpr void decrement()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(--base_ref(), void())

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
