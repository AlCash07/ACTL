// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/operation/equal.hpp>
#include "test.hpp"

TEST_CASE("ranges::equal")
{
    using SV = std::string_view;
    static_assert(!is_primary_overload_resolved_v<equal_f, SV, SV>);
    static_assert(
        std::is_same_v<
            ranges::equal_f,
            decltype(resolve_overload<SV, SV>(default_context{}, equal))>);
    // using OE = ac::expression<ac::equal_f, ac::type_f_t<char>,
    // ac::type_f_t<char>>; static_assert(std::is_same_v<bool,
    // detail::expr_result<OE, SV, SV>::type>);
    static_assert(
        std::is_same_v<bool, decltype(eval(ranges::equal(SV{}, SV{})))>);
    // static_assert(std::is_same_v<bool,
    // ac::resolved_result_type_t<ac::equal_f, SV, SV>>);
    // static_assert(std::is_same_v<bool,
    // ac::resolved_result_type_t<ac::equal_f, SV, SV>>); using T = typename
    // ac::resolved_result_type0<false, ac::equal_f, SV, SV>::type; using T =
    // typename ac::resolved_result_type0<false, ac::equal_f, SV, SV>::type;
    // using T1 = typename ac::resolved_result_type1<false, ac::equal_f, SV,
    // SV>::type; using T2 = typename ac::resolved_result_type1<false,
    // ac::equal_f, SV, SV>::type;
    static_assert(std::is_same_v<bool, decltype(eval(ac::equal(SV{}, SV{})))>);
    static_assert(std::is_same_v<
                  bool,
                  typename expression_result_type<decltype(ranges::equal(
                      SV{}, SV{}))>::type>);
    // static_assert(std::is_same_v<bool, typename resolved_result_type1<false,
    // equal_f, SV, SV>::type>); static_assert(std::is_same_v<bool, typename
    // resolved_result_type0<false, equal_f, SV, SV>::type>);
    // static_assert(std::is_same_v<bool, resolved_result_type_t<equal_f, SV,
    // SV>>);

    static_assert(!is_overload_resolved_v<default_context, equal_f, SV, SV>);

    // bool t = ac::equal("aba"sv, "aba"sv);
    // CHECK(ac::equal("aba"sv, "aba"sv));
    // CHECK(!ac::equal("aba"sv, "abc"sv));
    // CHECK(!ac::equal("aba"sv, "abac"sv));
    // CHECK(!ac::equal("abac"sv, "aba"sv));
}

// in instantiation of function template specialization
// 'ac::ranges::equal_f::evaluate< std::__1::basic_string_view<char,
// std::__1::char_traits<char> >, std::__1::basic_string_view<char,
// std::__1::char_traits<char> >, ac::expression<ac::equal_f,
// ac::type_f_t<char>, ac::type_f_t<char> > >' requested here
