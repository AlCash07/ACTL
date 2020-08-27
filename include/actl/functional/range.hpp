/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar.hpp>
#include <actl/range/traits.hpp>
#include <algorithm>

namespace ac::math {

struct range_tag {
    struct has_nested;
};

template <class T>
// struct category_impl<T, std::enable_if_t<is_range_v<T> && !is_contiguous_range_v<T>>> {
struct category_impl<T, std::enable_if_t<is_range_v<T>>> {
    using type = range_tag;
};

struct EqualRange {
    template <class EqualOp, class T, class U>
    static bool evaluate(const EqualOp& op, const T& lhs, const U& rhs) {
        return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs), op);
    }
};
constexpr operation_composer<EqualRange> equal_range;

template <class T, class U>
struct operation_resolver<Equal, range_tag, T, U> {
    static constexpr auto resolve(Equal op) { return equal_range(op.resolve_nested<T, U>()); }
};

struct LexicographicalCompareRange {
    template <class LessOp, class T, class U>
    static bool evaluate(const LessOp& op, const T& lhs, const U& rhs) {
        return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs),
                                            std::end(rhs), op);
    }
};
constexpr operation_composer<LexicographicalCompareRange> lexicographical_compare_range;

template <class T, class U>
struct operation_resolver<Less, range_tag, T, U> {
    static constexpr auto resolve(Less op) {
        return lexicographical_compare_range(op.resolve_nested<T, U>());
    }
};

}  // namespace ac::math
