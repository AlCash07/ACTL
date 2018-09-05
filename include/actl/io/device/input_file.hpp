/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/device/text_input_device.hpp>
#include <cstdio>

namespace ac {

template <bool Binary, uint32_t BufferSize = 1 << 11>
class input_file final : public input_device_base_t<Binary> {
public:
    input_file(std::FILE* file = stdin, bool line_buffered = true, bool take_ownership = false)
        : input_device_base_t<Binary>(buffer_, buffer_)
        , file_{file}
        , line_buffered_{line_buffered}
        , owner_{take_ownership} {}

    input_file(const char* file_name)
        : input_file(std::fopen(file_name, Binary ? "rb" : "r"), false, true) {}

    ~input_file() override {
        if (owner_) std::fclose(file_);
    }

private:
    void fill_buffer() override {
        this->head_ = buffer_;
        *buffer_ = '\0';
        if (EXPECT_TRUE(!line_buffered_)) {
            this->tail_ = this->head_ + std::fread(buffer_, 1, BufferSize, file_);
        } else {
            this->tail_ = this->head_;
            if (std::fgets(buffer_, BufferSize, file_)) {
                while (*this->tail_) ++this->tail_;
            }
        }
    }

    std::FILE* file_;
    const bool line_buffered_;
    const bool owner_;
    char       buffer_[BufferSize];
};

using binary_input_file = input_file<true>;
using text_input_file   = input_file<false>;

}  // namespace ac
