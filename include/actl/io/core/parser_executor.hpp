// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/function.hpp>
#include <actl/utility/index.hpp>

namespace ac::io {

template<class Parser>
bool parsed_available_data(Device auto& id, Parser& parser) {
    auto s = id.input_buffer();
    if (s.empty())
        return false;
    auto offset = parser.parse(s);
    id.move(static_cast<index>(offset));
    return offset == s.size();
}

template<class T, class Parser>
struct parser_executor {
    T& dst;
    Parser parser;

    bool operator()(Device auto& id) {
        while (parsed_available_data(id, parser)) {}
        bool ok = parser.ready();
        if (ok)
            dst = parser.value();
        return ok;
    }
};

template<class Parser>
struct parser_executor<void, Parser> {
    Parser parser;

    bool operator()(Device auto& id) {
        while (parsed_available_data(id, parser)) {}
        return parser.ready();
    }
};

template<class T, class P>
parser_executor(T&, P) -> parser_executor<T, P>;

template<class P>
parser_executor(P) -> parser_executor<void, P>;

} // namespace ac::io
