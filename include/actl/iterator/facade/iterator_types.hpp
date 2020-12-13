// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/iterator/traits.hpp>
#include <memory>

namespace ac {

namespace detail {

template <class Ref>
struct operator_arrow_dispatch {  // proxy references
    struct proxy {
        explicit proxy(const Ref& ref) : ref_{ref} {}
        Ref* operator->() { return std::addressof(ref_); }
        Ref ref_;
    };

    using type = proxy;

    static type apply(const Ref& x) { return type{x}; }
};

template <class T>
struct operator_arrow_dispatch<T&> {  // "real" references
    using type = T*;

    static type apply(T& x) { return std::addressof(x); }
};

template <>
struct operator_arrow_dispatch<void> {  // output iterator
    using type = void;
};

}  // namespace detail

template <
    class Category,
    class Value,
    class Reference = Value&,
    class Difference = std::ptrdiff_t>
struct iterator_types {
    using iterator_category = Category;
    using value_type = std::remove_cv_t<Value>;
    using reference = Reference;
    using pointer = typename detail::operator_arrow_dispatch<Reference>::type;
    using difference_type = Difference;
};

using output_iterator_types = iterator_types<std::output_iterator_tag, void, void, void>;

}  // namespace ac
