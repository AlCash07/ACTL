// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/dummy_map.hpp>
#include <actl/std/stack.hpp>

namespace ac::detail {

template<class Map, class Stack>
class component_stack {
    using T = map_key_t<Map>;

    Map m_map;
    Stack m_stack = {};
    map_value_t<Map> m_n = 0;

public:
    static_assert(std::is_same_v<T, typename Stack::value_type>);

    explicit component_stack(Map map) : m_map{map} {}

    void push(T x) {
        m_stack.push(x);
    }

    void pop(T last) {
        while (true) {
            T x = m_stack.top();
            m_stack.pop();
            put(m_map, x, m_n);
            if (x == last)
                break;
        }
        ++m_n;
    }

    template<class P>
    void pop_while(P pred) {
        while (!m_stack.empty()) {
            T& x = m_stack.top();
            if (!pred(x))
                break;
            put(m_map, x, m_n);
            m_stack.pop();
        }
        ++m_n;
    }
};

template<class S>
class component_stack<dummy_map, S> {
public:
    explicit component_stack(dummy_map) {}

    template<class T>
    void push(T) {}

    template<class T>
    void pop(T) {}

    template<class P>
    void pop_while(P) {}
};

template<class Map>
component_stack(Map&&) -> component_stack<Map, std::stack<map_key_t<Map>>>;

component_stack(dummy_map) -> component_stack<dummy_map, void>;

} // namespace ac::detail
