// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/range/iterator/interface/bidirectional_iterator_interface.hpp>

namespace ac {

/// Iterator over nodes of a singly linked list.
/// @tparam Node is required to provide the following:
/// - typename value_type;
/// - value(node);
/// - next(node) returning a pointer to the next node.
template<typename Node>
class singly_linked_list_iterator
    : public forward_iterator_interface<singly_linked_list_iterator<Node>> {
public:
    using value_type = Node::value_type;
    using difference_type = std::ptrdiff_t;

    singly_linked_list_iterator() = default;
    explicit constexpr singly_linked_list_iterator(Node& node) noexcept
        : m_node{&node} {}

    friend bool operator==(
        singly_linked_list_iterator, singly_linked_list_iterator
    ) = default;

    constexpr auto operator*() const {
        AC_ASSERT(m_node);
        return value(*m_node);
    }

    constexpr singly_linked_list_iterator& operator++() {
        AC_ASSERT(m_node);
        m_node = next(*m_node);
        return *this;
    }

private:
    Node* m_node = nullptr;
};

/// Iterator over nodes of a doubly linked list.
/// @tparam Node is required to provide the following:
/// - typename value_type;
/// - value(node);
/// - next(node) returning a pointer to the next node.
/// - prev(node) returning a pointer to the previous node.
template<typename Node>
class doubly_linked_list_iterator
    : public bidirectional_iterator_interface<
          doubly_linked_list_iterator<Node>> {
public:
    using value_type = Node::value_type;
    using difference_type = std::ptrdiff_t;

    doubly_linked_list_iterator() = default;
    explicit constexpr doubly_linked_list_iterator(Node& node) noexcept
        : m_node{&node} {}

    friend bool operator==(
        doubly_linked_list_iterator, doubly_linked_list_iterator
    ) = default;

    constexpr auto operator*() const {
        AC_ASSERT(m_node);
        return value(*m_node);
    }

    constexpr doubly_linked_list_iterator& operator++() {
        AC_ASSERT(m_node);
        m_node = next(*m_node);
        return *this;
    }

    constexpr doubly_linked_list_iterator& operator--() {
        AC_ASSERT(m_node);
        m_node = prev(*m_node);
        return *this;
    }

private:
    Node* m_node = nullptr;
};

} // namespace ac
