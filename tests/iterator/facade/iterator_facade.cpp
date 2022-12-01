// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/iterator/facade/iterator_facade.hpp>
#include <actl_test/iterator/iterator.hpp>
#include <actl_test/iterator/iterator_noexcept.hpp>

namespace {

template <class Category>
class int_iterator
    : public ac::iterator_facade<int_iterator<Category>, Category>
{
public:
    using value_type = int;

    explicit constexpr int_iterator() = default;

    explicit constexpr int_iterator(int value) noexcept : value_{value} {}

private:
    friend struct ac::iterator_core_access;

    constexpr int const& dereference() const noexcept
    {
        return value_;
    }

    constexpr void increment() noexcept
    {
        ++value_;
    }

    constexpr void decrement() noexcept
    {
        --value_;
    }

    constexpr void advance(int n) noexcept
    {
        value_ += n;
    }

    constexpr bool equals(int_iterator const& rhs) const noexcept
    {
        return value_ == rhs.value_;
    }

    constexpr int distance_to(int_iterator const& rhs) const noexcept
    {
        return rhs.value_ - value_;
    }

    int value_ = 0;
};

} // namespace

TEST_CASE("iterator_facade")
{
    SECTION(
        "iterator_facade provides full standard-compliant interface for each "
        "iterator category")
    {
        test_iterator(
            int_iterator<std::input_iterator_tag>{2},
            std::input_iterator_tag{});
        test_iterator(
            int_iterator<std::forward_iterator_tag>{2},
            std::forward_iterator_tag{});
        test_iterator(
            int_iterator<std::bidirectional_iterator_tag>{2},
            std::bidirectional_iterator_tag{});
        test_random_access_iterator(
            int_iterator<std::random_access_iterator_tag>{2},
            int_iterator<std::random_access_iterator_tag>{5});
    }

    constexpr int_iterator<std::random_access_iterator_tag> iter{2};
    // iterator_facade propagates noexcept
    test_iterator_noexcept(iter, std::random_access_iterator_tag{});
}
