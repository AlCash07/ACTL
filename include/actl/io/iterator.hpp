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
struct input_device_iterator_types
{
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using reference = T const&;
    using difference_type = use_default;
};

/// Iterator similar to
/// http://en.cppreference.com/w/cpp/iterator/istream_iterator
template <class T, class Device>
class input_device_iterator
    : public iterator_facade<
          input_device_iterator<T, Device>,
          input_device_iterator_types<T>>
{
public:
    explicit input_device_iterator() = default;

    explicit input_device_iterator(Device& device) : device_{&device}
    {
        increment();
    }

    input_device_iterator(input_device_iterator const&) = default;

private:
    friend struct ac::iterator_core_access;

    T const& dereference() const
    {
        return value_;
    }

    void increment()
    {
        AC_ASSERT(device_);
        if (!io::read(*device_, value_))
            device_ = nullptr;
    }

    bool equals(input_device_iterator const& rhs) const
    {
        return device_ == rhs.device_;
    }

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
    : public iterator_facade<
          output_device_iterator<T, Device>,
          output_iterator_types>
{
public:
    explicit output_device_iterator(Device& device) : device_{&device} {}

    output_device_iterator(output_device_iterator const&) = default;

    void operator=(T const& value) const
    {
        io::write(*device_, value);
    }

private:
    friend struct ac::iterator_core_access;

    output_device_iterator const& dereference() const
    {
        return *this;
    }

    void increment() {}

    Device* device_;
};

template <class T, class Device>
auto make_output_device_iterator(Device& device)
{
    return output_device_iterator<T, Device>{device};
}

} // namespace ac
