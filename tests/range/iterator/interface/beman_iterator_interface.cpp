// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/iterator/interface/iterator_adaptor.hpp>
#include <actl/range/iterator/interface/random_access_iterator_interface.hpp>
#include <algorithm>
#include "test.hpp"

namespace ac {

struct repeated_chars_iterator
    : random_access_iterator_interface<repeated_chars_iterator> {
    using value_type = char;

    constexpr repeated_chars_iterator()
        : m_chars{nullptr}, m_size{0}, m_index{0} {}
    constexpr repeated_chars_iterator(
        char const* chars, std::ptrdiff_t size, std::ptrdiff_t index
    )
        : m_chars{chars}, m_size{size}, m_index{index} {}

    constexpr char operator*() const {
        return m_chars[m_index % m_size];
    }
    constexpr repeated_chars_iterator& operator+=(std::ptrdiff_t offset) {
        m_index += offset;
        return *this;
    }
    constexpr auto operator-(repeated_chars_iterator const& that) const {
        return this->m_index - that.m_index;
    }

private:
    char const* m_chars;
    std::ptrdiff_t m_size;
    std::ptrdiff_t m_index;
};

TEST_CASE("repeated_chars_iterator") {
    // 3 is the length of "foo", 0 is this iterator's position.
    constexpr repeated_chars_iterator first{"foo", 3, 0};
    // Same as above, but now the iterator's position is 7.
    constexpr repeated_chars_iterator last{"foo", 3, 7};
    /* iteration */ {
        std::string result;
        std::copy(first, last, std::back_inserter(result));
        CHECK(result == "foofoof");
    }
    static_assert(std::distance(first, last) == 7);
    static_assert(std::next(first, 7) == last);
    /* concepts */ {
        // Note: Check each sub-concept to get the less verbose error
        // message first!
        static_assert(std::input_iterator<repeated_chars_iterator>);
        static_assert(std::forward_iterator<repeated_chars_iterator>);
        static_assert(std::bidirectional_iterator<repeated_chars_iterator>);
        static_assert(std::random_access_iterator<repeated_chars_iterator>);
    }
}

template<typename Predicate>
struct filtered_int_iterator
    : iterator_adaptor<
          filtered_int_iterator<Predicate>,
          int*,
          std::forward_iterator_tag> {
    filtered_int_iterator() = default;

    filtered_int_iterator(int* iter, int* last, Predicate pred)
        : iterator_adaptor<
              filtered_int_iterator<Predicate>,
              int*,
              std::forward_iterator_tag>{iter}
        , m_last{last}
        , m_pred{std::move(pred)} {
        this->base_ref() = std::find_if(this->base_ref(), m_last, m_pred);
    }

    // A forward iterator based on iterator_interface usually requires
    // three user-defined operations. Since we are adapting an existing
    // iterator (an int *), we only need to define this one. The others are
    // implemented by iterator_adaptor, using the underlying int*.
    filtered_int_iterator& operator++() {
        this->base_ref() =
            std::find_if(std::next(this->base_ref()), m_last, m_pred);
        return *this;
    }

private:
    int* m_last;
    Predicate m_pred;
};

TEST_CASE("filtered_int_iterator") {
    int a[] = {1, 2, 3, 4};
    filtered_int_iterator f{
        std::begin(a),
        std::end(a),
        [](int i) { return (i % 2) == 0; },
    };
    REQUIRE(*f == 2);
    f++; // doesn't compile with std::iterator_interface
    REQUIRE(*f == 4);
}

struct class_with_member_function {
    int f() {
        return 3;
    }
};

struct always_iterator : random_access_iterator_interface<always_iterator> {
    using value_type = class_with_member_function;

    always_iterator() : m_index{0} {}
    always_iterator(std::ptrdiff_t index) : m_index{index} {}

    class_with_member_function operator*() const {
        return m_value;
    }
    always_iterator& operator+=(std::ptrdiff_t offset) {
        m_index += offset;
        return *this;
    }
    auto operator-(always_iterator const& that) const {
        return this->m_index - that.m_index;
    }

private:
    class_with_member_function m_value;
    std::ptrdiff_t m_index;
};

TEST_CASE("operator-> works as expected") {
    always_iterator ai;
    CHECK(ai->f() == 3);
}

struct dummy_input_iterator : input_iterator_interface<dummy_input_iterator> {
    using value_type = int;
    using difference_type = std::ptrdiff_t;

    constexpr dummy_input_iterator() = default;

    dummy_input_iterator(const dummy_input_iterator&) = delete;
    dummy_input_iterator& operator=(const dummy_input_iterator&) = delete;
    dummy_input_iterator(dummy_input_iterator&&) = default;
    dummy_input_iterator& operator=(dummy_input_iterator&&) = default;

    constexpr auto& operator*() const {
        return foo;
    }
    constexpr dummy_input_iterator& operator++() {
        return *this;
    }

    friend constexpr bool
    operator==(std::default_sentinel_t const&, dummy_input_iterator const&) {
        return true;
    }

    int foo = 0;
};

static_assert(std::input_iterator<dummy_input_iterator>);
static_assert(!std::forward_iterator<dummy_input_iterator>);

} // namespace ac
