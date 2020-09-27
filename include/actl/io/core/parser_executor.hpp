/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

namespace ac::io {

template <class T, class Parser>
class parser_executor {
    template <class Device>
    bool parsed_available(Device& id) {
        auto s = id.input_data();
        if (s.empty())
            return false;
        auto offset = parser.parse(s);
        id.move(offset);
        return offset == s.size();
    }

    bool finalize() {
        bool ok = parser.ready();
        if (ok)
            dst = parser.value();
        return ok;
    }

public:
    T& dst;
    Parser parser;

    template <class Device>
    bool operator()(Device& id) {
        while (parsed_available(id))
            ;
        return finalize();
    }
};

template <class T, class P>
parser_executor(T&, P) -> parser_executor<T, P>;

}  // namespace ac::io
