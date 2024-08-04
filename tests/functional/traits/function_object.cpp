// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/traits/all.hpp>
#include <functional>
#include "test.hpp"

namespace {

struct function_object {
    int operator()();
};

static_assert(0ul == ac::arity_v<function_object>);
static_assert(0ul == ac::arity_v<function_object&>);
static_assert(0ul == ac::arity_v<function_object&&>);
static_assert(std::is_same_v<int, ac::return_t<function_object>>);
static_assert(!ac::is_noexcept_v<function_object>);

struct function_object_noexcept {
    int operator()() noexcept;
};

static_assert(0ul == ac::arity_v<function_object_noexcept>);
static_assert(std::is_same_v<int, ac::return_t<function_object_noexcept>>);
static_assert(ac::is_noexcept_v<function_object_noexcept>);

struct const_function_object {
    int operator()() const;
};

static_assert(0ul == ac::arity_v<const_function_object>);
static_assert(0ul == ac::arity_v<const_function_object const>);
static_assert(0ul == ac::arity_v<const_function_object const&>);
static_assert(std::is_same_v<int, ac::return_t<const_function_object const>>);
static_assert(!ac::is_noexcept_v<const_function_object>);

struct const_function_object_noexcept {
    int operator()() const noexcept;
};

static_assert(0ul == ac::arity_v<const_function_object_noexcept>);
static_assert(std::
                  is_same_v<int, ac::return_t<const_function_object_noexcept>>);
static_assert(ac::is_noexcept_v<const_function_object_noexcept>);

struct function_object_params {
    void const* operator()(int const*, int&&, int) const noexcept;
};

static_assert(3ul == ac::arity_v<function_object_params>);
static_assert(3ul == ac::arity_v<function_object_params&>);
static_assert(3ul == ac::arity_v<function_object_params&&>);
static_assert(std::
                  is_same_v<void const*, ac::return_t<function_object_params>>);
static_assert(std::is_same_v<
              int const*,
              ac::parameter_at_t<0, function_object_params>>);
static_assert(std::is_same_v<
              int&&,
              ac::parameter_at_t<1, function_object_params>>);
static_assert(std::is_same_v<
              int,
              ac::parameter_at_t<2, function_object_params>>);
static_assert(ac::is_noexcept_v<function_object_params>);

} // namespace

using std_function = std::function<int*(int&)>;
static_assert(1ul == ac::arity_v<std_function>);
static_assert(std::is_same_v<int*, ac::return_t<std_function>>);
static_assert(std::is_same_v<int&, ac::parameter_at_t<0, std_function>>);
