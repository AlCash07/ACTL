// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <iterator>

namespace ac {

// https://en.cppreference.com/w/cpp/iterator/input_or_output_iterator
template<typename Iter>
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
            requires { typename Iter::value_type; },
            "nested `value_type` has to be specified by the iterator"
        );
        if constexpr (!std::is_base_of_v<
                          std::random_access_iterator_tag,
                          typename Iter::iterator_category>)
            static_assert(
                requires { typename Iter::difference_type; },
                "nested `difference_type` has to be specified by the iterator "
                "that doesn't provide random access"
            );
    }

    // Post-increment is a free function here so that it's not hidden by the
    // pre-increment operator++() defined by the derived iterator.
    // TODO: std::input_or_output_iterator doesn't require the return type here
    // to be exactly Iter
    // https://en.cppreference.com/w/cpp/iterator/weakly_incrementable
    friend constexpr Iter operator++(Iter& iter, int) noexcept(
        noexcept(Iter{iter}, ++iter)
    ) {
        Iter iter_copy = iter;
        ++iter;
        return iter_copy;
    }
};

} // namespace ac
