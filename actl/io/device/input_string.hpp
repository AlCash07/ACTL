/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/device/text_input_device.hpp>

namespace ac {

// Picks up data appended to the string but doesn't handle reallocation.
template <bool Binary>
class input_string final : public input_device_base_t<Binary> {
public:
    input_string(const char* str) : input_device_base_t<Binary>(str, str + std::strlen(str)) {}

    input_string(const std::string& str)
        : input_device_base_t<Binary>(str.data(), str.data() + str.size()) {}

private:
    void fill_buffer() override {
        while (*this->tail_) ++this->tail_;
    }
};

using binary_input_string = input_string<true>;
using text_input_string   = input_string<false>;

}  // namespace ac
