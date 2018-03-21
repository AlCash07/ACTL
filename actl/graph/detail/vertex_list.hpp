/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/container/container_traits.hpp>
#include <actl/type/none.hpp>

namespace ac::detail {

template <class VC, class T = typename VC::value_type, bool RA = is_random_access<VC>::value>
class vertex_list {
public:
    int size() const { return static_cast<int>(vertices_.size()); }

    auto begin() const { return vertices_.begin(); }
    auto end()   const { return vertices_.end(); }

private:
    VC vertices_;
};

template <class VC, class T>
class vertex_list<VC, T, true> {
public:
    int size() const { return static_cast<int>(vertices_.size()); }

    void resize(int n) { vertices_.resize(n); }

    int begin() const { return 0; }
    int end()   const { return size(); }

private:
    VC vertices_;
};

template <class VC>
class vertex_list<VC, none, true> {
public:
    int size() const { return n_; }

    void resize(int n) { n_ = n; }

    int begin() const { return 0; }
    int end()   const { return size(); }

private:
    int n_;
};

}  // namespace ac::detail
