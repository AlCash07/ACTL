/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/device/text_output_device.hpp>

namespace ac {

template <bool Binary, uint32_t BufferSize = 1 << 11>
class output_string final : public output_device_base_t<Binary, BufferSize> {
public:
    output_string(std::string& str) : str_{str} {}

    ~output_string() override { this->flush(); }

private:
    void flush_buffer(int count) override { str_.append(this->buffer_, count); }

    std::string& str_;
};

using binary_output_string = output_string<true>;
using text_output_string   = output_string<false>;

}  // namespace ac
