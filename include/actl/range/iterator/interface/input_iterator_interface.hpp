// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/noexcept/deduce_noexcept_and_return.hpp>
#include <actl/range/iterator/interface/basic_iterator_interface.hpp>
#include <actl/range/iterator/interface/detail/arrow_operator.hpp>

namespace ac {

// https://en.cppreference.com/w/cpp/iterator/input_iterator
template <class Iter>
class input_iterator_interface : public basic_iterator_interface<Iter> {
public:
    using iterator_category = std::input_iterator_tag;

    constexpr auto operator->() const
        AC_DEDUCE_NOEXCEPT_AND_RETURN(detail::arrow_operator(*derived()))

protected:
    constexpr Iter const& derived() const noexcept {
        return static_cast<Iter const&>(*this);
    }

    constexpr Iter& derived() noexcept {
        return static_cast<Iter&>(*this);
    }
};

} // namespace ac
