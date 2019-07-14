/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/numeric/compare.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/std/utility.hpp>
#include <actl/util/span.hpp>
#include <actl/util/type_traits.hpp>
#include <memory>

namespace ac {

namespace detail {

template <class Data, class Dimensions>
class ndarray_base;

/* Helper types. */

template <index...>
struct static_array {
    index operator[](index) const {
        ACTL_ASSERT(false);
        return -1;
    }
};

template <index I0, index... Is>
struct static_array<I0, Is...> {
    constexpr index operator[](index i) const {
        return i == 0 ? I0 : static_array<Is...>{}[i - 1];
    }
};

template <index...>
struct static_product : index_constant<1> {};

template <index D0, index... Ds>
struct static_product<D0, Ds...> : index_constant<D0 * static_product<Ds...>::value> {};

template <class T, index N>
struct nd_initializer_list {
    using type = std::initializer_list<typename nd_initializer_list<T, N - 1>::type>;
};

template <class T>
struct nd_initializer_list<T, 0> {
    using type = T;
};

template <class T>
struct nd_initializer_list<T, -1> {
    using type = none;
};

template <class T, index N>
using nd_initializer_list_t = typename nd_initializer_list<T, N>::type;

template <class Int>
inline index compute_product(const cspan<Int>& x) {
    index res = 1;
    for (auto v : x) res *= v;
    return res;
}

/* NDArray container class, supports array and std::unique_ptr as data. */

template <class Data>
class ndarray_container;

template <class T, index Size>
class ndarray_container<T[Size]> {
public:
    using value_type = T;

    ndarray_container(index size) { ACTL_ASSERT(size == Size); }

    T* data() { return data_; }
    const T* data() const { return data_; }

    void swap(ndarray_container& rhs) { std::swap(data_, rhs.data_); }

private:
    T data_[Size];
};

template <class T>
class ndarray_container<std::unique_ptr<T[]>> {
public:
    using value_type = T;

    ndarray_container(index size) : data_{std::make_unique<T[]>(static_cast<size_t>(size))} {}

    T* data() { return data_.get(); }
    const T* data() const { return data_.get(); }

    void swap(ndarray_container& rhs) { std::swap(data_, rhs.data_); }

private:
    std::unique_ptr<T[]> data_;
};

/* NDArray data class, supports container or pointer as data. */

template <index N, class Data>
class ndarray_data : public ndarray_container<Data> {
    using base_t = ndarray_container<Data>;
    using T = value_t<base_t>;

public:
    ndarray_data(index size) : base_t{size} {}

    template <class InIter>
    ndarray_data(index size, InIter it) : base_t{size} {
        std::copy_n(it, size, this->data());
    }

    ndarray_data(index size, const T& value) : base_t{size} {
        std::fill_n(this->data(), size, value);
    }

    template <class Dims>
    ndarray_data(index size, nd_initializer_list_t<T, N> il, Dims dims) : base_t{size} {
        index strides[std::max(N - 1, (index)1)];  // array of size 0 is not standard-compliant
        if constexpr (N >= 2) {
            strides[N - 2] = dims[N - 1];
            for (index i = N - 3; i >= 0; --i)
                strides[i] = strides[i + 1] * dims[i];
        }
        T* end = initialize<0>(this->data(), il, dims, strides);
        std::fill(end, this->data() + size, T{});
    }

    void swap(ndarray_data& rhs) { base_t::swap(rhs); }

private:
    template <index I, class Dims>
    T* initialize(T* data, nd_initializer_list_t<T, N - I> il, Dims dims, const index* strides) {
        ACTL_ASSERT(il.size() <= static_cast<size_t>(dims[I]));
        if constexpr (I + 1 < N) {
            for (auto it : il) {
                T* end = initialize<I + 1>(data, it, dims, strides);
                data += strides[I];
                std::fill(end, data, T{});
            }
            return data;
        } else {
            return copy(il, data);
        }
    }
};

template <index N, class T>
class ndarray_data<N, T*> {
public:
    using value_type = T;

    ndarray_data(index, T* data) : data_{data} {}

    void swap(ndarray_data& rhs) { std::swap(data_, rhs.data_); }

    T* data() { return data_; }
    const T* data() const { return data_; }

private:
    T* data_;
};

/* NDArray shape class, manages dimensions. */

// This class is needed to guarantee dims construction before data to avoid undefined behavior.
template <class Dims>
struct ndarray_dims {
    Dims dims_;
};

template <class Int, index N>
struct ndarray_dims<Int[N]> {
    ndarray_dims() = default;
    ndarray_dims(std::initializer_list<Int> dims) { std::copy_n(dims.begin(), N, dims_); }

    Int dims_[N] = {};
};

template <index N, class Data, class Dims>
class ndarray_shape : private ndarray_dims<Dims>, public ndarray_data<N, Data> {
    using base_dims = ndarray_dims<Dims>;
    using base_data = ndarray_data<N, Data>;
    using Int = value_t<Dims>;
    using T = value_t<base_data>;

    using base_dims::dims_;

public:
    template <class... Ts>
    explicit ndarray_shape(const Dims& dims, Ts... args)
        : base_dims{dims}, base_data{size(), args...} {}

    template <class... Ts>
    explicit ndarray_shape(Dims&& dims, Ts... args)
        : base_dims{std::move(dims)}, base_data{size(), args...} {}

    template <class... Ts>
    explicit ndarray_shape(std::conditional_t<N == 1, Int, std::initializer_list<Int>> dims,
                           Ts... args)
        : base_dims{dims}, base_data{size(), args...} {}

    explicit ndarray_shape(nd_initializer_list_t<T, N> il)
        : base_dims{}, base_data{(compute_dimensions<0>(il), size()), il, std::data(dims_)} {}

    constexpr index rank() const { return static_cast<index>(std::size(dims_)); }

    index size() const { return compute_product<Int>(dims_); }

    const Dims& dimensions() const { return dims_; }

    void swap(ndarray_shape& rhs) {
        using std::swap;
        swap(dims_, rhs.dims_);
        base_data::swap(rhs);
    }

private:
    template <index I>
    void compute_dimensions(nd_initializer_list_t<T, N - I> il) {
        if constexpr (I < N) {
            smax(dims_[I], static_cast<Int>(il.size()));
            for (auto i : il) compute_dimensions<I + 1>(i);
        }
    }
};

template <index N, class Data, index... Ds>
class ndarray_shape<N, Data, static_array<Ds...>> : public ndarray_data<N, Data> {
    using base_t = ndarray_data<N, Data>;
    using Dims = static_array<Ds...>;

public:
    template <class... Ts>
    explicit ndarray_shape(Ts... args) : base_t{size(), args...} {}

    template <bool B = 0 < N, enable_int_if<B> = 0>
    explicit ndarray_shape(nd_initializer_list_t<value_t<base_t>, N> il)
        : base_t{size(), il, dimensions()} {}

    static constexpr index rank() { return static_size_v<Dims>; }

    static constexpr index size() { return static_product<Ds...>::value; }

    static constexpr Dims dimensions() { return {}; }
};

/* NDArray subscript operator implementation */

template <class T, index N, class Dims, bool = 1 < N>
struct ndarray_reference {
    using Int = value_t<Dims>;
    using type = ndarray_base<T*, span<Int, N - 1>>;

    template <class NDArrayPtr>
    static type get(NDArrayPtr ptr, index i) {
        span<Int, N - 1> dims{std::data(ptr->dimensions()) + 1, N - 1};
        return type{dims, ptr->data() + i * compute_product(dims)};
    }
};

template <class T, index N, index D0, index... Ds>
struct ndarray_reference<T, N, static_array<D0, Ds...>, true> {
    using type = ndarray_base<T*, static_array<Ds...>>;

    template <class NDArrayPtr>
    static type get(NDArrayPtr ptr, index i) {
        return type{ptr->data() + i * static_product<Ds...>::value};
    }
};

template <class T, class Dims>
struct ndarray_reference<T, 1, Dims, false> {
    using type = T&;

    template <class NDArrayPtr>
    static type get(NDArrayPtr ptr, index i) {
        return ptr->data()[i];
    }
};

template <class T, index N, class Dims>
using ndarray_reference_t = typename ndarray_reference<T, N, Dims>::type;

template <index N, class Data, class Dims>
class ndarray_subscript : public ndarray_shape<N, Data, Dims> {
    using base_t = ndarray_shape<N, Data, Dims>;
    using T = value_t<base_t>;

public:
    using reference       = ndarray_reference_t<T, N, Dims>;
    using const_reference = ndarray_reference_t<const T, N, Dims>;

    using base_t::base_t;

    constexpr auto dimension(index i) const {
        ACTL_ASSERT(0 <= i && i < this->rank());
        return this->dimensions()[static_cast<size_type_t<Dims>>(i)];
    }

    reference operator[](index i) {
        ACTL_ASSERT(0 <= i && i < dimension(0));
        return ndarray_reference<T, N, Dims>::get(this, i);
    }

    const_reference operator[](index i) const {
        ACTL_ASSERT(0 <= i && i < dimension(0));
        return ndarray_reference<const T, N, Dims>::get(this, i);
    }
};

template <class Data, class Dims>
class ndarray_subscript<0, Data, Dims> : public ndarray_shape<0, Data, static_array<>> {
    using base_t = ndarray_shape<0, Data, static_array<>>;
    using T = value_t<base_t>;

public:
    using base_t::base_t;

    constexpr operator T&() { return *this->data(); }
    constexpr operator const T&() const { return *this->data(); }
};

/* Base class, defines type aliases and secondary interface methods. */

template <class Data, class Dims>
class ndarray_base : public ndarray_subscript<static_size_v<Dims>, Data, Dims> {
    using base_t = ndarray_subscript<static_size_v<Dims>, Data, Dims>;
    using T = value_t<base_t>;

public:
    using size_type              = index;
    using difference_type        = index;
    using reference              = T&;
    using const_reference        = const T&;
    using pointer                = T*;
    using const_pointer          = const T*;
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using base_t::base_t;

    iterator begin() { return this->data(); }
    iterator end() { return begin() + this->size(); }

    const_iterator begin() const { return this->data(); }
    const_iterator end() const { return begin() + this->size(); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }

    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend() const { return rend(); }

    bool empty() const { return this->size() == 0; }

    template <class... Ints>
    reference operator()(Ints... is) {
        return this->data()[getIndex<0>(0, is...)];
    }

    template <class... Ints>
    const_reference operator()(Ints... is) const {
        return this->data()[getIndex<0>(0, is...)];
    }

private:
    template <index I>
    index getIndex(index res) const {
        ACTL_ASSERT(I == this->rank());
        return res;
    }

    template <index I, class... Ints>
    index getIndex(index res, index i, Ints... is) const {
        ACTL_ASSERT(0 <= i && i < this->dimension(I));
        if constexpr (I > 0) res *= this->dimension(I);
        return getIndex<I + 1>(res + i, is...);
    }
};

template <class D0, class S0, class D1, class S1>
inline bool operator == (const ndarray_base<D0, S0>& lhs, const ndarray_base<D1, S1>& rhs) {
    if (lhs.rank() != rhs.rank()) return false;
    for (index i = 0; i < lhs.rank(); ++i) {
        if (lhs.dimension(i) != rhs.dimension(i)) return false;
    }
    return std::equal(lhs.data(), lhs.data() + lhs.size(), rhs.data());
}

template <class D0, class S0, class D1, class S1>
inline bool operator != (const ndarray_base<D0, S0>& lhs, const ndarray_base<D1, S1>& rhs) {
    return !(lhs == rhs);
}

template <class D, class S>
inline void swap(ndarray_base<D, S>& lhs, ndarray_base<D, S>& rhs) {
    lhs.swap(rhs);
}

template <class T, index... Ds>
using ndarray_base_static = ndarray_base<T[static_product<Ds...>::value], static_array<Ds...>>;

template <index N>
struct dimensions {
    using type = int[N];
};

template <>
struct dimensions<0> {
    using type = static_array<>;
};

template <>
struct dimensions<dynamic_size> {
    using type = std::vector<int>;
};

template <index N>
using dimensions_t = typename dimensions<N>::type;

}  // namespace detail

template <index... Is>
struct static_size<detail::static_array<Is...>> : index_constant<sizeof...(Is)> {};

/**
 * N-dimensional array with fixed dimensions.
 */
template <class T, index... Dimensions>
class ndarray_static : public detail::ndarray_base_static<T, Dimensions...> {
    using base_t = detail::ndarray_base_static<T, Dimensions...>;

public:
    using base_t::base_t;
};

/**
 * N-dimensional array.
 */
template <class T, index N = dynamic_size, class Dims = detail::dimensions_t<N>>
class ndarray : public detail::ndarray_base<std::unique_ptr<T[]>, Dims> {
    using base_t = detail::ndarray_base<std::unique_ptr<T[]>, Dims>;

public:
    using base_t::base_t;
};

template <class T, class Dims>
class ndarray<T, 0, Dims> : public ndarray_static<T> {
public:
    using ndarray_static<T>::ndarray_static;
};

/**
 * View of an N-dimensional array.
 */
template <class T, index N = dynamic_size, class Dims = detail::dimensions_t<N>>
class ndarray_view : public detail::ndarray_base<T*, Dims> {
    using base_t = detail::ndarray_base<T*, Dims>;

public:
    using base_t::base_t;

    template <class Data, class Dimensions>
    ndarray_view& operator = (const detail::ndarray_base<Data, Dimensions>& rhs) {
        ACTL_ASSERT(this->size() == rhs.size());
        copy(*this, rhs.begin());
        return *this;
    }
};

}  // namespace ac
