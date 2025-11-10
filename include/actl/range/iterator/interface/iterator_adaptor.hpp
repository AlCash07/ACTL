// Iterator adaptor is designed to simplify creation of similar iterators.
// Adopted from boost:
// http://www.boost.org/doc/libs/latest/libs/iterator/doc/iterator_adaptor.html
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/iterator/interface/iterator_interface_selector.hpp>
#include <actl/range/iterator/traits/category.hpp>

namespace ac {

template<
    typename DerivedIterator,
    typename AdaptedIterator,
    typename Category = iter_category_t<AdaptedIterator>>
class iterator_adaptor
    : public iterator_interface_selector_t<DerivedIterator, Category> {
    using base_t = iterator_interface_selector_t<DerivedIterator, Category>;

protected:
    constexpr AdaptedIterator& base_ref() noexcept {
        return m_base;
    }

public:
    // These are the default aliases that can be overwritten in the
    // DerivedIterator iterator when needed.
    using value_type = std::iter_value_t<AdaptedIterator>;
    using difference_type = std::iter_difference_t<AdaptedIterator>;

    // Default constructor is useful because
    // std::sentinel_for requires std::semiregular.
    template<typename Iter = AdaptedIterator>
        requires std::is_default_constructible_v<Iter>
    constexpr iterator_adaptor(
    ) noexcept(std::is_nothrow_default_constructible_v<AdaptedIterator>)
        : m_base{Iter{}} {}

    explicit constexpr iterator_adaptor(AdaptedIterator const& iter
    ) noexcept(noexcept(AdaptedIterator{iter}))
        : m_base{iter} {}

    constexpr AdaptedIterator const& base() const noexcept {
        return m_base;
    }

    constexpr decltype(auto) operator*() const AC_DEDUCE_NOEXCEPT_AND_RETURN( //
        *base()
    )

    constexpr DerivedIterator& operator++() AC_DEDUCE_NOEXCEPT_AND_RETURN( //
        ++base_ref(),
        base_t::derived()
    )

    template<typename C = Category>
        requires std::is_base_of_v<std::bidirectional_iterator_tag, C>
    constexpr DerivedIterator& operator--() AC_DEDUCE_NOEXCEPT_AND_RETURN( //
        --base_ref(),
        base_t::derived()
    )

    template<typename Difference>
    // AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN didn't work for some reason.
        requires requires(AdaptedIterator& iter, Difference offset) {
            iter += offset;
        }
    constexpr DerivedIterator& operator+=(Difference offset)
        AC_DEDUCE_NOEXCEPT_AND_RETURN( //
            base_ref() += offset,
            base_t::derived()
        )

    // TODO: make this a hidden friend.
    template<typename DerivedIter1, typename AdaptedIter1, typename Category1>
    constexpr auto operator==(
        iterator_adaptor<DerivedIter1, AdaptedIter1, Category1> const& that
    ) const AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN( //
        this->base() == that.base()
    )

    template<typename DerivedIter1, typename AdaptedIter1, typename Category1>
    constexpr auto operator-(
        iterator_adaptor<DerivedIter1, AdaptedIter1, Category1> const& that
    ) const AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN( //
        this->base() - that.base()
    )

private:
    AdaptedIterator m_base;
};

} // namespace ac
