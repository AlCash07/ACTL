// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/iterator/facade/operator_arrow_dispatch.hpp>
#include <actl/iterator/traits.hpp>
#include <actl/traits/nested_or_default.hpp>

namespace ac {

NESTED_OR_DEFAULT(reference)
NESTED_OR_DEFAULT(difference)

template <class Category, class Value, class Reference = Value&, class Difference = std::ptrdiff_t>
struct iterator_types {
    using iterator_category = Category;
    using value_type = std::remove_cv_t<Value>;
    using reference = Reference;
    using pointer = typename detail::operator_arrow_dispatch<Reference>::type;
    using difference_type = Difference;
};

struct output_iterator_types {
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using reference = void;
    using difference_type = void;
};

}  // namespace ac
