// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/iterator/interface/basic_iterator_interface.hpp>

namespace ac {

template <class Iter>
class output_iterator_interface : public basic_iterator_interface<Iter> {
public:
    using iterator_category = std::output_iterator_tag;
};

} // namespace ac
