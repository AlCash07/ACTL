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
#include <actl/traits/type_traits.hpp>

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
    using iterator = std::conditional_t<is_const, typename T::const_iterator, typename T::iterator>;
};

}  // namespace detail

template <class Container>
class container_range : public range_facade<container_range<Container>,
                                            typename detail::cr_traits<Container>::iterator>,
                        protected detail::cr_traits<Container>::base {
    using It = typename detail::cr_traits<Container>::iterator;

public:
    static constexpr bool writable = !detail::cr_traits<Container>::is_const;

    template <class... Ts>
    explicit container_range(Ts&&... args)
        : detail::cr_traits<Container>::base{{std::forward<Ts>(args)...}} {}

    It begin() const { return this->data_.begin(); }
    It end() const { return this->data_.end(); }
};

template <class Container>
inline auto make_range(Container&& container) {
    return container_range<remove_rvalue_ref_t<Container>>(std::forward<Container>(container));
}

template <class Container>
inline auto make_crange(const Container& container) {
    return make_range(container);
}

}  // namespace ac
