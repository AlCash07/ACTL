/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/iterator/iterator_facade.hpp>

namespace ac {

/**
 * Iterator similar to http://en.cppreference.com/w/cpp/iterator/istream_iterator
 */
template <class T, class Device>
class input_device_iterator
    : public iterator_facade<input_device_iterator<T, Device>,
                             std::input_iterator_tag,
                             T,
                             const T&,
                             const T*> {
public:
    explicit input_device_iterator() = default;

    explicit input_device_iterator(Device& device) : device_{&device} {
        increment();
    }

    input_device_iterator(const input_device_iterator&) = default;

private:
    friend struct iterator_core_access;

    const T& dereference() const { return value_; }

    void increment() {
        ACTL_ASSERT(device_);
        if (!read(*device_, value_)) device_ = nullptr;
    }

    bool equals(const input_device_iterator& rhs) const { return device_ == rhs.device_; }

    Device* device_;
    T value_;
};

template <class T, class Device>
inline auto make_input_device_iterator(Device& device) {
    return input_device_iterator<T, Device>(device);
}

/**
 * Iterator similar to http://en.cppreference.com/w/cpp/iterator/ostream_iterator
 */
template <class T, class Device>
class output_device_iterator
    : public iterator_facade<output_device_iterator<T, Device>,
                             std::output_iterator_tag,
                             T,
                             const output_device_iterator<T, Device>&,
                             T*,
                             void> {
public:
    explicit output_device_iterator(Device& device) : device_{&device} {}

    output_device_iterator(const output_device_iterator&) = default;

    void operator = (const T& value) const {
        write(*device_, value);
    }

private:
    friend struct iterator_core_access;

    const output_device_iterator& dereference() const { return *this; }

    void increment() {}

    Device* device_;
};

template <class T, class Device>
inline auto make_output_device_iterator(Device& device) {
    return output_device_iterator<T, Device>(device);
}

}  // namespace ac
