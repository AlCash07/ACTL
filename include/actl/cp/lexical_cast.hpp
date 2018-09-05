/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <sstream>
#include <string>

namespace ac {

namespace detail {

template <class T, class U>
struct lexical_cast_impl;

template <class U>
struct lexical_cast_impl<std::string, U> {
    static std::string doit(const U& value) {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
};

template <class T>
struct lexical_cast_impl<T, std::string> {
    static T doit(const std::string& str) {
        return lexical_cast_impl<T, char*>::doit(str.data());
    }
};

template <class T>
struct lexical_cast_impl<T, char*> {
    static T doit(const char* str) {
        T res = T();
        std::stringstream ss(str);
        if (!(ss >> res) || !ss.eof())
            throw std::exception();
        return res;
    }
};

}  // namespace detail

template <class T, class U>
inline T lexical_cast(const U& value) {
    return ac::detail::lexical_cast_impl<T, std::decay_t<U>>::doit(value);
}

}  // namespace ac