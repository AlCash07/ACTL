// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/container/conversion/to_constant.hpp>
#include <actl/numeric/interval/interval_kind.hpp>
#include <actl/utility/none.hpp>

namespace ac {

template <
    class Begin,
    class End = Begin,
    class Kind = constant<interval_kind::closed_open>>
struct interval
{
    /*[[no_unique_address]]*/ Begin begin;
    /*[[no_unique_address]]*/ End end;
    /*[[no_unique_address]]*/ Kind kind;

    template <
        class BeginT,
        class EndT,
        bool B = !is_none_v<Begin> && !is_none_v<End>,
        enable_int_if<B> = 0>
    constexpr interval(BeginT&& begin, EndT&& end, Kind kind = {})
        : begin{std::forward<BeginT>(begin)}
        , end{std::forward<EndT>(end)}
        , kind{kind}
    {}

    template <
        bool B = !is_none_v<Begin> && is_none_v<End>,
        class BeginT,
        enable_int_if<B> = 0>
    constexpr interval(BeginT&& begin, Kind kind = {})
        : begin{std::forward<BeginT>(begin)}, kind{kind}
    {}

    template <
        bool B = is_none_v<Begin> && !is_none_v<End>,
        enable_int_if<B> = 0,
        class EndT>
    constexpr interval(EndT&& end, Kind kind = {})
        : end{std::forward<EndT>(end)}, kind{kind}
    {}

    template <bool B = is_none_v<Begin>&& is_none_v<End>, enable_int_if<B> = 0>
    constexpr interval(Kind kind = {}) : kind{kind}
    {}
};

namespace detail {

template <class T>
struct is_interval : std::false_type
{};

template <class Begin, class End, class Kind>
struct is_interval<interval<Begin, End, Kind>> : std::true_type
{};

template <class T>
using enable_int_if_interval =
    enable_int_if<is_interval<remove_cvref_t<T>>::value>;

} // namespace detail

template <class Interval, detail::enable_int_if_interval<Interval> = 0>
constexpr decltype(auto) interval_begin(Interval&& interval) noexcept
{
    return std::forward<Interval>(interval).begin;
}

template <class Interval, detail::enable_int_if_interval<Interval> = 0>
constexpr decltype(auto) interval_end(Interval&& interval) noexcept
{
    return std::forward<Interval>(interval).end;
}

template <class Interval, detail::enable_int_if_interval<Interval> = 0>
constexpr decltype(auto) interval_kind(Interval&& interval) noexcept
{
    return std::forward<Interval>(interval).kind;
}

} // namespace ac
