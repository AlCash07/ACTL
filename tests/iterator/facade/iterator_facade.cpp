#include <actl/iterator/facade/iterator_facade.hpp>
#include "iterator/test_iterator.hpp"
#include "iterator/test_iterator_noexcept.hpp"

namespace {

template <class Category>
struct int_iterator_types
{
    using iterator_category = Category;
    using value_type = int;
    using reference = const int&;
    using difference_type = int;
};

template <class Category>
class int_iterator
    : public iterator_facade<
          int_iterator<Category>,
          int_iterator_types<Category>>
{
public:
    explicit constexpr int_iterator() = default;

    explicit constexpr int_iterator(int value) noexcept : value_{value} {}

private:
    friend struct ac::iterator_core_access;

    constexpr const int& dereference() const noexcept
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

    constexpr bool equals(const int_iterator& rhs) const noexcept
    {
        return value_ == rhs.value_;
    }

    constexpr int distance_to(const int_iterator& rhs) const noexcept
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

    // iterator_facade propagates noexcept
    test_iterator_noexcept(
        int_iterator<std::random_access_iterator_tag>{2},
        std::random_access_iterator_tag{});
}
