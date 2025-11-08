// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <iterator>

namespace ac {

/// Avoids boilerplate needed to satisfy full requirements of
/// <a
/// href="https://en.cppreference.com/w/cpp/iterator/input_or_output_iterator">
/// std::input_or_output_iterator</a>.
template<typename DerivedIterator>
class basic_iterator_interface {
public:
    // This constructor exists for a sole purpose of
    // providing a clear compilation error message
    // when the required nested types aren't specified.
    //
    // We cannot static_assert in the class scope,
    // because those nested types are defined in a derived class.
    //
    // Constructor is a good place for the asserts,
    // because it's the only interface function
    // which is guaranteed to be called.
    constexpr basic_iterator_interface() noexcept {
        static_assert(
            requires { typename DerivedIterator::value_type; },
            "nested `value_type` has to be specified by the iterator"
        );
        if constexpr (!std::is_base_of_v<
                          std::random_access_iterator_tag,
                          typename DerivedIterator::iterator_category>)
            static_assert(
                requires { typename DerivedIterator::difference_type; },
                "nested `difference_type` has to be specified by the iterator "
                "that doesn't provide random access"
            );
    }

    // Post-increment is a free function here so that it's not hidden by the
    // pre-increment operator++() defined by the derived iterator.
    //
    // It has to be a template instead of a regular function taking
    // `DerivedIterator&` parameter to provide implementations with different
    // return types depending if the type is copyable. We cannot simply check
    // `std::copyable<DerivedIterator>`, because DerivedIterator is not fully
    // formed here, because it's a derived class.
    template<std::same_as<DerivedIterator> Iter>
    friend constexpr DerivedIterator operator++(Iter& iter, int) noexcept(
        noexcept(DerivedIterator{iter}, ++iter)
    ) {
        DerivedIterator iter_copy = iter;
        ++iter;
        return iter_copy;
    }

    // Post-increment implementation for non-copyable types
    // such as input iterators following the standard proposal
    // https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2024/p2727r4.html#changes-since-r1
    template<std::same_as<DerivedIterator> Iter>
        requires(!std::copyable<Iter>)
    friend constexpr void operator++(Iter& iter, int) noexcept(noexcept(++iter)
    ) {
        ++iter;
    }
};

} // namespace ac
