// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/numeric/interval/interval.hpp>

namespace ac {

template <class Begin, class End = Begin>
using open_interval = interval<Begin, End, constant<interval_kind::open>>;

template <class Begin, class End = Begin>
using open_closed_interval =
    interval<Begin, End, constant<interval_kind::open_closed>>;

template <class Begin, class End = Begin>
using closed_open_interval =
    interval<Begin, End, constant<interval_kind::closed_open>>;

template <class Begin, class End = Begin>
using closed_interval = interval<Begin, End, constant<interval_kind::closed>>;

template <class Begin>
using open_unbounded_interval =
    interval<Begin, none, constant<interval_kind::open_unbounded>>;

template <class Begin>
using closed_unbounded_interval =
    interval<Begin, none, constant<interval_kind::closed_unbounded>>;

template <class End>
using unbounded_open_interval =
    interval<none, End, constant<interval_kind::unbounded_open>>;

template <class End>
using unbounded_closed_interval =
    interval<none, End, constant<interval_kind::unbounded_closed>>;

using unbounded_interval =
    interval<none, none, constant<interval_kind::unbounded>>;

template <class Begin, class End = Begin>
using any_interval = interval<Begin, End, interval_kind>;

} // namespace ac
