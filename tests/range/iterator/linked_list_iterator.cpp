// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/range/iterator/linked_list_iterator.hpp>
#include <actl_test/iterator/iterator.hpp>
#include "test.hpp"

struct node_single {
    using value_type = int;

    int value;
    node_single* next;
};

struct node_double {
    using value_type = int;

    int value;
    node_double* next;
    node_double* prev;
};

template<typename Node>
auto value(Node& node) {
    return node.value;
}
template<typename Node>
auto next(Node& node) {
    return node.next;
}
template<typename Node>
auto prev(Node& node) {
    return node.prev;
}

TEST_CASE("singly_linked_list_iterator") {
    node_single node1{1, nullptr};
    node_single node0{0, &node1};
    ac::singly_linked_list_iterator iter{node0};
    ac::test_iterator(iter, std::forward_iterator_tag{});
    CHECK(*iter == 0);
    ++iter;
    CHECK(*iter == 1);
    ++iter;
    CHECK(iter == ac::singly_linked_list_iterator<node_single>{});
}

TEST_CASE("doubly_linked_list_iterator") {
    node_double node1{1, nullptr, nullptr};
    node_double node0{0, &node1, nullptr};
    node1.prev = &node0;
    ac::doubly_linked_list_iterator iter{node0};
    ac::test_iterator(iter, std::bidirectional_iterator_tag{});
    CHECK(*iter == 0);
    ++iter;
    CHECK(*iter == 1);
    --iter;
    CHECK(*iter == 0);
    ++iter;
    ++iter;
    CHECK(iter == ac::doubly_linked_list_iterator<node_double>{});
}
