// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/io/io.hpp>
#include <actl/iterator/facade/iterator_facade.hpp>

namespace ac {

template <class T>
struct input_device_iterator_types {
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using reference = const T&;
    using difference_type = use_default;
};

/// Iterator similar to http://en.cppreference.com/w/cpp/iterator/istream_iterator
template <class T, class Device>
class input_device_iterator
    : public iterator_facade<input_device_iterator<T, Device>, input_device_iterator_types<T>> {
public:
    explicit input_device_iterator() = default;

    explicit input_device_iterator(Device& device) : device_{&device} {
        increment();
    }

    input_device_iterator(const input_device_iterator&) = default;

private:
    friend struct ac::iterator_core_access;

    const T& dereference() const {
        return value_;
    }

    void increment() {
        ACTL_ASSERT(device_);
        if (!io::read(*device_, value_))
            device_ = nullptr;
    }

    bool equals(const input_device_iterator& rhs) const {
        return device_ == rhs.device_;
    }

    Device* device_;
    T value_;
};

template <class T, class Device>
auto make_input_device_iterator(Device& device) {
    return input_device_iterator<T, Device>{device};
}

/// Iterator similar to http://en.cppreference.com/w/cpp/iterator/ostream_iterator
template <class T, class Device>
class output_device_iterator
    : public iterator_facade<output_device_iterator<T, Device>, output_iterator_types> {
public:
    explicit output_device_iterator(Device& device) : device_{&device} {}

    output_device_iterator(const output_device_iterator&) = default;

    void operator=(const T& value) const {
        io::write(*device_, value);
    }

private:
    friend struct ac::iterator_core_access;

    const output_device_iterator& dereference() const {
        return *this;
    }

    void increment() {}

    Device* device_;
};

template <class T, class Device>
auto make_output_device_iterator(Device& device) {
    return output_device_iterator<T, Device>{device};
}

} // namespace ac
