// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/traits/callable_traits.hpp>
#include <functional>
#include "test.hpp"

namespace {

template<class Fn, bool IsNoexcept>
void test_void_free_function_traits() {
    static_assert(0ul == ac::arity_v<Fn>);
    static_assert(std::is_same_v<void, ac::return_t<Fn>>);
    static_assert(IsNoexcept == ac::is_noexcept_v<Fn>);

    using RawFn = std::remove_cvref_t<Fn>;
    static_assert(ac::FreeFunction<RawFn>);
    static_assert(!ac::MemberFunction<RawFn>);
}

} // namespace

TEST_CASE("void 0 arity free function: value, pointer, references") {
    test_void_free_function_traits<void(), false>();
    test_void_free_function_traits<void() noexcept, true>();
    test_void_free_function_traits<void (*)(), false>();
    test_void_free_function_traits<void (*)() noexcept, true>();
    test_void_free_function_traits<void (&)(), false>();
    test_void_free_function_traits<void (&)() noexcept, true>();
    test_void_free_function_traits<void (*const&)(), false>();
    test_void_free_function_traits<void (*const&)() noexcept, true>();
    test_void_free_function_traits<void (&&)(), false>();
    test_void_free_function_traits<void (&&)() noexcept, true>();
}

using free_function_params =
    int && (int, int const, int&, int const&, int*, int const*) noexcept;
static_assert(6ul == ac::arity_v<free_function_params>);
static_assert(std::is_same_v<int&&, ac::return_t<free_function_params>>);
static_assert(std::is_same_v<int, ac::parameter_at_t<0, free_function_params>>);
static_assert(std::is_same_v<int, ac::parameter_at_t<1, free_function_params>>);
static_assert(std::
                  is_same_v<int&, ac::parameter_at_t<2, free_function_params>>);
static_assert(std::is_same_v<
              int const&,
              ac::parameter_at_t<3, free_function_params>>);
static_assert(std::
                  is_same_v<int*, ac::parameter_at_t<4, free_function_params>>);
static_assert(std::is_same_v<
              int const*,
              ac::parameter_at_t<5, free_function_params>>);
static_assert(ac::is_noexcept_v<free_function_params>);

static_assert(ac::FreeFunction<free_function_params>);
static_assert(!ac::MemberFunction<free_function_params>);

/* Direct access to ac::callable_traits */
using FnTraits = ac::callable_traits<free_function_params>;
static_assert(6ul == FnTraits::arity);
static_assert(std::is_same_v<int&&, typename FnTraits::return_type>);
static_assert(std::is_same_v<
              int const*,
              typename FnTraits::template parameter_at<5>>);
static_assert(FnTraits::is_noexcept);

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

namespace {

struct fn_object {
    S operator()();
};

static_assert(0ul == ac::arity_v<fn_object>);
static_assert(0ul == ac::arity_v<fn_object&>);
static_assert(0ul == ac::arity_v<fn_object&&>);
static_assert(std::is_same_v<S, ac::return_t<fn_object>>);
static_assert(!ac::is_noexcept_v<fn_object>);

static_assert(!ac::FreeFunction<fn_object>);
static_assert(!ac::MemberFunction<fn_object>);

struct fn_object_noexcept {
    S operator()() noexcept;
};

static_assert(0ul == ac::arity_v<fn_object_noexcept>);
static_assert(std::is_same_v<S, ac::return_t<fn_object_noexcept>>);
static_assert(ac::is_noexcept_v<fn_object_noexcept>);

struct const_fn_object {
    S operator()() const;
};

static_assert(0ul == ac::arity_v<const_fn_object>);
static_assert(0ul == ac::arity_v<const_fn_object const>);
static_assert(0ul == ac::arity_v<const_fn_object const&>);
static_assert(std::is_same_v<S, ac::return_t<const_fn_object const>>);
static_assert(!ac::is_noexcept_v<const_fn_object>);

struct const_fn_object_noexcept {
    S operator()() const noexcept;
};

static_assert(0ul == ac::arity_v<const_fn_object_noexcept>);
static_assert(std::is_same_v<S, ac::return_t<const_fn_object_noexcept>>);
static_assert(ac::is_noexcept_v<const_fn_object_noexcept>);

struct fn_object_params {
    void const* operator()(S const*, int&&, S) const noexcept;
};

static_assert(3ul == ac::arity_v<fn_object_params>);
static_assert(3ul == ac::arity_v<fn_object_params&>);
static_assert(3ul == ac::arity_v<fn_object_params&&>);
static_assert(std::is_same_v<void const*, ac::return_t<fn_object_params>>);
static_assert(std::
                  is_same_v<S const*, ac::parameter_at_t<0, fn_object_params>>);
static_assert(std::is_same_v<int&&, ac::parameter_at_t<1, fn_object_params>>);
static_assert(std::is_same_v<S, ac::parameter_at_t<2, fn_object_params>>);
static_assert(ac::is_noexcept_v<fn_object_params>);

static_assert(!ac::FreeFunction<fn_object_params>);
static_assert(!ac::MemberFunction<fn_object_params>);

/* Direct access to ac::function_object_traits */
using FnObjTraits = ac::function_object_traits<fn_object_noexcept>;
static_assert(0ul == FnObjTraits::arity);
static_assert(std::is_same_v<S, typename FnObjTraits::return_type>);
static_assert(FnObjTraits::is_noexcept);

} // namespace

using std_function = std::function<int*(int&)>;
static_assert(1ul == ac::arity_v<std_function>);
static_assert(std::is_same_v<int*, ac::return_t<std_function>>);
static_assert(std::is_same_v<int&, ac::parameter_at_t<0, std_function>>);

static_assert(!ac::FreeFunction<std_function>);
static_assert(!ac::MemberFunction<std_function>);
