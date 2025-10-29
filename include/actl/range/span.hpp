// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/static_size.hpp>
#include <actl/range/interface/contiguous_range_interface.hpp>
#include <actl/range/traits/concepts.hpp>
#include <actl/range/traits/properties.hpp>
#include <actl/range/utility/size_holder.hpp>

namespace ac {

template<class T, size_t N = dynamic_size>
class span : public contiguous_range_interface<span<T, N>> {
public:
    using element_type = T;

    static constexpr size_t extent = N;

    template<size_t M = N>
        requires(M == 0 || M == dynamic_size)
    constexpr span() : m_storage{nullptr, 0} {}

    constexpr span(T* ptr, size_t count) : m_storage{ptr, count} {}

    constexpr span(T* first, T* last)
        : span{first, static_cast<size_t>(last - first)} {}

    constexpr span(ContiguousRange auto&& r)
        : span{ranges::data(r), ranges::size(r)} {}

    constexpr T* data() const {
        return m_storage.data;
    }

    constexpr size_t size() const {
        return m_storage.size();
    }

    constexpr span<T> first(size_t n) const {
        AC_ASSERT(n <= size());
        return {data(), n};
    }

    constexpr span<T> last(size_t n) const {
        AC_ASSERT(n <= size());
        return {this->end() - n, n};
    }

    constexpr span<T> subspan(size_t offset, size_t count) const {
        AC_ASSERT(offset + count <= size());
        return {data() + offset, count};
    }

    constexpr span<T> subspan(size_t offset) const {
        return subspan(offset, static_cast<size_t>(size() - offset));
    }

private:
    struct storage_t : size_holder<N> {
        T* data;
        constexpr storage_t(T* ptr, size_t count)
            : size_holder<N>{count}, data{ptr} {
            AC_ASSERT(ptr || count == 0);
        }
    };

    storage_t m_storage;
};

template<ContiguousRange R>
span(R&&) -> span<
    std::remove_pointer_t<decltype(ranges::data(std::declval<R>()))>,
    static_size_v<R>>;

template<class T, size_t N>
struct range_properties<span<T, N>> : default_range_properties {};

template<class T, size_t N = dynamic_size>
using cspan = span<T const, N>;

template<class S, class T>
struct is_span : std::false_type {};

template<class T, size_t N>
struct is_span<span<T, N>, T> : std::true_type {};

} // namespace ac
