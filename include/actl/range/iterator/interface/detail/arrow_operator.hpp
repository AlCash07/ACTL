// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_AND_RETURN.hpp>
#include <memory>

namespace ac::detail {

// "real" references
template<class T>
constexpr T* arrow_operator(T& x) noexcept {
    return std::addressof(x);
}

// output iterator
constexpr void arrow_operator(void) noexcept {}

template<class T>
class arrow_proxy {
public:
    template<class Arg>
    explicit arrow_proxy(Arg&& x) noexcept(noexcept(T{std::forward<Arg>(x)}))
        : m_value{std::forward<Arg>(x)} {}

    T* operator->() noexcept {
        return std::addressof(m_value);
    }

private:
    T m_value;
};

// operator->() needs special support for input iterators to strictly meet the
// standard's requirements. If *i is not a reference type, we must still
// produce an lvalue to which a pointer can be formed. We do that by
// returning a proxy object containing an instance of the reference object.
template<class Ref>
constexpr auto arrow_operator(Ref&& ref)
    AC_DEDUCE_NOEXCEPT_AND_RETURN(arrow_proxy<std::remove_cvref_t<Ref>>{
        std::forward<Ref>(ref)
    })

} // namespace ac::detail
