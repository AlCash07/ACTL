// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/io/io.hpp>
#include <actl/range/iterator/interface/input_iterator_interface.hpp>
#include <actl/range/iterator/interface/output_iterator_interface.hpp>

namespace ac {

/// Iterator similar to
/// http://en.cppreference.com/w/cpp/iterator/istream_iterator
template <class T, class Device>
class input_device_iterator
    : public input_iterator_interface<input_device_iterator<T, Device>>
{
public:
    using value_type = T;

    explicit input_device_iterator() = default;

    explicit input_device_iterator(Device& device) : device_{&device}
    {
        ++*this;
    }

    input_device_iterator(input_device_iterator const&) = default;

    T const& operator*() const noexcept
    {
        return value_;
    }

    input_device_iterator& operator++()
    {
        AC_ASSERT(device_);
        if (!io::read(*device_, value_))
            device_ = nullptr;
        return *this;
    }

private:
    Device* device_;
    T value_;
};

template <class T, class Device>
auto make_input_device_iterator(Device& device)
{
    return input_device_iterator<T, Device>{device};
}

/// Iterator similar to
/// http://en.cppreference.com/w/cpp/iterator/ostream_iterator
template <class T, class Device>
class output_device_iterator
    : public output_iterator_interface<output_device_iterator<T, Device>>
{
public:
    explicit output_device_iterator(Device& device) : device_{&device} {}

    output_device_iterator(output_device_iterator const&) = default;

    void operator=(T const& value) const
    {
        io::write(*device_, value);
    }

    output_device_iterator const& operator*() const noexcept
    {
        return *this;
    }

    output_device_iterator& operator++()
    {
        return *this;
    }

private:
    Device* device_;
};

template <class T, class Device>
auto make_output_device_iterator(Device& device)
{
    return output_device_iterator<T, Device>{device};
}

} // namespace ac
