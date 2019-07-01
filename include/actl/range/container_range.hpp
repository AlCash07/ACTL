/***************************************************************************************************
 * Container (possibly with const and/or reference) with range interface.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/range/range_facade.hpp>
#include <actl/util/type_traits.hpp>

namespace ac {

namespace detail {

template <class C, bool Mutable = false>
struct cr_data {
    C data_;
};

template <class C>
struct cr_data<C, true> {
    mutable C data_;
};

template <class C>
struct cr_traits {
    static constexpr bool is_const = std::is_const_v<std::remove_reference_t<C>>;

    using base     = cr_data<C, !(is_const || std::is_reference_v<C>)>;
    using T        = remove_cvref_t<C>;
    using iterator = iterator_t<T>;
};

}  // namespace detail

template <class Container>
class container_range
    : public range_facade<container_range<Container>, iterator_t<detail::cr_traits<Container>>>,
      protected detail::cr_traits<Container>::base {
    using It = iterator_t<detail::cr_traits<Container>>;

public:
    static constexpr bool writable = !detail::cr_traits<Container>::is_const;

    template <class... Ts>
    explicit container_range(Ts&&... args)
        : detail::cr_traits<Container>::base{{std::forward<Ts>(args)...}} {}

    It begin() const { return this->data_.begin(); }
    It end() const { return this->data_.end(); }
};

template <class Container>
inline auto make_range(Container&& cont) {
    return container_range<Container>{std::forward<Container>(cont)};
}

template <class Container>
inline auto make_crange(const Container& cont) {
    return make_range(cont);
}

}  // namespace ac
