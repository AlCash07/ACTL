// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/dummy_map.hpp>
#include <actl/std/stack.hpp>

namespace ac::detail {

template <class Map, class Stack>
class component_stack {
    using T = map_key_t<Map>;

    Map map_;
    Stack stack_ = {};
    map_value_t<Map> n_ = 0;

public:
    static_assert(std::is_same_v<T, value_type_t<Stack>>);

    explicit component_stack(Map map) : map_{map} {}

    void push(T x) {
        stack_.push(x);
    }

    void pop(T last) {
        while (true) {
            T x = stack_.top();
            stack_.pop();
            put(map_, x, n_);
            if (x == last)
                break;
        }
        ++n_;
    }

    template <class P>
    void pop_while(P pred) {
        while (!stack_.empty()) {
            T& x = stack_.top();
            if (!pred(x))
                break;
            put(map_, x, n_);
            stack_.pop();
        }
        ++n_;
    }
};

template <class S>
class component_stack<dummy_map, S> {
public:
    explicit component_stack(dummy_map) {}

    template <class T>
    void push(T) {}

    template <class T>
    void pop(T) {}

    template <class P>
    void pop_while(P) {}
};

template <class Map>
component_stack(Map&&) -> component_stack<Map, std::stack<map_key_t<Map>>>;

component_stack(dummy_map)->component_stack<dummy_map, void>;

} // namespace ac::detail
