// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/function_traits.hpp>
#include "test.hpp"

namespace {

struct S {
    int member_function();
    int member_function_noexcept() noexcept;
    int const_member_function() const;
    int const_member_function_noexcept() const noexcept;

    int reference_member_function() &;
    int reference_member_function_noexcept() & noexcept;
    int const_reference_member_function() const&;
    int const_reference_member_function_noexcept() const& noexcept;

    int rvalue_reference_member_function() &&;
    int rvalue_reference_member_function_noexcept() && noexcept;
    int const_rvalue_reference_member_function() const&&;
    int const_rvalue_reference_member_function_noexcept() const&& noexcept;

    int const& member_function_params(int const, int&&) const noexcept;
};

template<class MemberFn, class ClassParam, bool IsNoexcept>
void test_member_function_traits() {
    static_assert(1ul == ac::arity_v<MemberFn>);
    static_assert(std::is_same_v<int, ac::return_t<MemberFn>>);
    static_assert(std::is_same_v<ClassParam, ac::parameter_at_t<0, MemberFn>>);
    static_assert(IsNoexcept == ac::is_noexcept_v<MemberFn>);

    using RawMemberFn = std::remove_cvref_t<MemberFn>;
    static_assert(!ac::FreeFunction<RawMemberFn>);
    static_assert(ac::MemberFunction<RawMemberFn>);
}

} // namespace

TEST_CASE("member function pointers") {
    test_member_function_traits<decltype(&S::member_function), S&, false>();
    test_member_function_traits<
        decltype(&S::member_function_noexcept),
        S&,
        true>();
    test_member_function_traits<
        decltype(&S::const_member_function),
        S const&,
        false>();
    test_member_function_traits<
        decltype(&S::const_member_function_noexcept),
        S const&,
        true>();
    test_member_function_traits<
        decltype(&S::reference_member_function),
        S&,
        false>();
    test_member_function_traits<
        decltype(&S::reference_member_function_noexcept),
        S&,
        true>();
    test_member_function_traits<
        decltype(&S::const_reference_member_function),
        S const&,
        false>();
    test_member_function_traits<
        decltype(&S::const_reference_member_function_noexcept),
        S const&,
        true>();
    test_member_function_traits<
        decltype(&S::rvalue_reference_member_function),
        S&&,
        false>();
    test_member_function_traits<
        decltype(&S::rvalue_reference_member_function_noexcept),
        S&&,
        true>();
    test_member_function_traits<
        decltype(&S::const_rvalue_reference_member_function),
        S const&&,
        false>();
    test_member_function_traits<
        decltype(&S::const_rvalue_reference_member_function_noexcept),
        S const&&,
        true>();
    /* qualifying a member function pointer doesn't change its traits */
    test_member_function_traits<
        decltype(&S::member_function_noexcept) const&,
        S&,
        true>();
    test_member_function_traits<
        decltype(&S::member_function_noexcept)&&,
        S&,
        true>();
}

using mem_fn_params = decltype(&S::member_function_params);
static_assert(3ul == ac::arity_v<mem_fn_params>);
static_assert(std::is_same_v<int const&, ac::return_t<mem_fn_params>>);
static_assert(std::is_same_v<S const&, ac::parameter_at_t<0, mem_fn_params>>);
static_assert(std::is_same_v<int, ac::parameter_at_t<1, mem_fn_params>>);
static_assert(std::is_same_v<int&&, ac::parameter_at_t<2, mem_fn_params>>);
static_assert(ac::is_noexcept_v<mem_fn_params>);

static_assert(!ac::FreeFunction<mem_fn_params>);
static_assert(ac::MemberFunction<mem_fn_params>);
