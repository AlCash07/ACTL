// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/deduce_noexcept.hpp>
#include <memory>

namespace ac::detail {

// operator->() needs special support for input iterators to strictly meet the
// standard's requirements. If *i is not a reference type, we must still
// produce an lvalue to which a pointer can be formed. We do that by
// returning a proxy object containing an instance of the reference object.
template <class Ref>
struct operator_arrow_dispatch
{
    struct proxy
    {
        explicit proxy(const Ref& ref) noexcept(noexcept(Ref{ref})) : ref_{ref}
        {}

        Ref* operator->() noexcept
        {
            return std::addressof(ref_);
        }

        Ref ref_;
    };

    using type = proxy;

    static type apply(const Ref& x) AC_DEDUCE_NOEXCEPT_AND_RETURN(type{x})
};

template <class T>
struct operator_arrow_dispatch<T&> // "real" references
{
    using type = T*;

    static type apply(T& x) noexcept
    {
        return std::addressof(x);
    }
};

template <>
struct operator_arrow_dispatch<void> // output iterator
{
    using type = void;
};

} // namespace ac::detail
