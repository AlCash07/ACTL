/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/text_format.hpp>

namespace ac::io {

template <class Device, class Format = text_format<Device::mode>>
class formatted : public Device, public Format {
public:
    using Device::Device;

    Format& format() { return *this; }
};

}  // namespace ac::io
