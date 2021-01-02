// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <iterator>

namespace ac {

struct output_iterator_types {
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using reference = void;
    using difference_type = void;
};

}  // namespace ac
