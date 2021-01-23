// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/utility/use_default.hpp>
#include <iterator>

namespace ac {

struct default_iterator_types {
    using reference = use_default;
    using difference_type = use_default;
};

struct default_iterator_adaptor_types {
    using iterator_category = use_default;
    using value_type = use_default;
    using reference = use_default;
    using difference_type = use_default;
};

struct output_iterator_types {
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using reference = void;
    using difference_type = void;
};

}  // namespace ac
