// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/assert.hpp>
#include <actl/container/array/semi_static_array.hpp>
#include <actl/functional/range/comparison.hpp>
#include <actl/functional/scalar/basic_math/max.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/std/utility.hpp>
#include <actl/traits/type_traits.hpp>
#include <actl/utility/span.hpp>
#include <memory>

namespace ac {

namespace detail {

template <class Data, class Dimensions>
class tensor_base;

template <class T, index N>
struct nd_initializer_list {
    using type = std::initializer_list<typename nd_initializer_list<T, N - 1>::type>;
};

template <class T>
struct nd_initializer_list<T, 0> {
    using type = T;
};

template <class T>
struct nd_initializer_list<T, dynamic_size> {
    struct type {};
};

template <class T, index N>
using nd_initializer_list_t = typename nd_initializer_list<T, N>::type;

template <index... Is>
constexpr index static_product_v = (1 * ... * Is);

template <class Int>
index compute_product(const cspan<Int>& x) {
    index res = 1;
    for (auto v : x)
        res *= v;
    return res;
}

/* NDArray container class, supports array and std::unique_ptr as data. */

template <class Data>
class tensor_container;

template <class T, size_t Size>
class tensor_container<std::array<T, Size>> {
public:
    using value_type = T;

    tensor_container(index size) {
        ACTL_ASSERT(size == index{Size});
    }

    T* data() {
        return data_.data();
    }
    const T* data() const {
        return data_.data();
    }

    void swap(tensor_container& rhs) {
        std::swap(data_, rhs.data_);
    }

private:
    std::array<T, Size> data_;
};

template <class T>
class tensor_container<std::unique_ptr<T[]>> {
public:
    using value_type = T;

    tensor_container(index size) : data_{new T[static_cast<size_t>(size)]} {}

    T* data() {
        return data_.get();
    }
    const T* data() const {
        return data_.get();
    }

    void swap(tensor_container& rhs) {
        std::swap(data_, rhs.data_);
    }

private:
    std::unique_ptr<T[]> data_;
};

/* NDArray data class, supports container or pointer as data. */

template <index N, class Data>
class tensor_data : public tensor_container<Data> {
    using base_t = tensor_container<Data>;
    using T = value_type_t<base_t>;

public:
    tensor_data(index size) : base_t{size} {}

    template <class InRange>
    tensor_data(index size, InRange data) : base_t{size} {
        ACTL_ASSERT(size == static_cast<index>(std::size(data)));
        std::copy_n(std::begin(data), size, this->data());
    }

    tensor_data(index size, const T& value) : base_t{size} {
        std::fill_n(this->data(), size, value);
    }

    template <class Dims>
    tensor_data(index size, const nd_initializer_list_t<T, N>& il, Dims dims) : base_t{size} {
        index strides[std::max(N - 1, (index)1)];  // array of size 0 is not standard-compliant
        if constexpr (N >= 2) {
            strides[N - 2] = dims[N - 1];
            for (index i = N - 3; i >= 0; --i)
                strides[i] = strides[i + 1] * dims[i];
        }
        T* end = initialize<0>(this->data(), il, dims, strides);
        std::fill(end, this->data() + size, T{});
    }

    void swap(tensor_data& rhs) {
        base_t::swap(rhs);
    }

private:
    template <index I, class Dims>
    T* initialize(T* ptr, const nd_initializer_list_t<T, N - I>& il, Dims dims,
                  const index* strides) {
        ACTL_ASSERT(il.size() <= static_cast<size_t>(dims[I]));
        if constexpr (I + 1 < N) {
            for (const auto& x : il) {
                T* end = initialize<I + 1>(ptr, x, dims, strides);
                ptr += strides[I];
                std::fill(end, ptr, T{});
            }
            return ptr;
        } else {
            return std::copy(il.begin(), il.end(), ptr);
        }
    }
};

template <index N, class T>
class tensor_data<N, T*> {
public:
    using value_type = T;

    tensor_data(index, T* ptr) : ptr_{ptr} {}

    T* data() {
        return ptr_;
    }
    const T* data() const {
        return ptr_;
    }

    void swap(tensor_data& rhs) {
        std::swap(ptr_, rhs.ptr_);
    }

private:
    T* ptr_;
};

/* NDArray shape class, manages dimensions. */

// This class is needed to guarantee dims construction before data to avoid undefined behavior.
template <class Dims>
struct tensor_dims {
    Dims dims_;
};

template <class Int, index N>
struct tensor_dims<std::array<Int, N>> {
    tensor_dims() = default;
    tensor_dims(std::initializer_list<Int> dims) {
        std::copy_n(dims.begin(), N, dims_.begin());
    }

    std::array<Int, N> dims_ = {};
};

template <index N, class Data, class Dims>
class tensor_shape : private tensor_dims<Dims>, public tensor_data<N, Data> {
    using base_dims = tensor_dims<Dims>;
    using base_data = tensor_data<N, Data>;
    using Int = value_type_t<Dims>;
    using T = value_type_t<base_data>;

    using base_dims::dims_;

public:
    template <class... Ts>
    explicit tensor_shape(const Dims& dims, Ts... args)
        : base_dims{dims}, base_data{size(), args...} {}

    template <class... Ts>
    explicit tensor_shape(Dims&& dims, Ts... args)
        : base_dims{std::move(dims)}, base_data{size(), args...} {}

    template <class... Ts>
    explicit tensor_shape(std::conditional_t<N == 1, Int, std::initializer_list<Int>> dims,
                          Ts... args)
        : base_dims{dims}, base_data{size(), args...} {}

    explicit tensor_shape(nd_initializer_list_t<T, N> il)
        : base_dims{}, base_data{(compute_dimensions<0>(il), size()), il, std::data(dims_)} {}

    constexpr index rank() const {
        return static_cast<index>(std::size(dims_));
    }

    index size() const {
        return compute_product<Int>(dims_);
    }

    const Dims& dimensions() const {
        return dims_;
    }

    void swap(tensor_shape& rhs) {
        using std::swap;
        swap(dims_, rhs.dims_);
        base_data::swap(rhs);
    }

private:
    template <index I>
    void compute_dimensions(nd_initializer_list_t<T, N - I> il) {
        if constexpr (I < N) {
            max(inplace(dims_[I]), static_cast<Int>(il.size()));
            for (auto i : il)
                compute_dimensions<I + 1>(i);
        }
    }
};

template <index N, class Data, index... Ds>
class tensor_shape<N, Data, static_array<index, Ds...>> : public tensor_data<N, Data> {
    using base_t = tensor_data<N, Data>;
    using Dims = static_array<index, Ds...>;

public:
    template <class... Ts>
    explicit tensor_shape(Ts... args) : base_t{size(), args...} {}

    template <bool B = 0 < N, enable_int_if<B> = 0>
    explicit tensor_shape(nd_initializer_list_t<value_type_t<base_t>, N> il)
        : base_t{size(), il, dimensions()} {}

    static constexpr index rank() {
        return static_size_v<Dims>;
    }

    static constexpr index size() {
        return static_product_v<Ds...>;
    }

    static constexpr Dims dimensions() {
        return {};
    }
};

/* NDArray subscript operator implementation */

template <class T, index N, class Dims, bool = 1 < N>
struct tensor_reference {
    using Int = value_type_t<Dims>;
    using type = tensor_base<T*, span<Int, N - 1>>;

    template <class TensorPtr>
    static type get(TensorPtr ptr, index i) {
        span<Int, N - 1> dims{std::data(ptr->dimensions()) + 1, N - 1};
        return type{dims, ptr->data() + i * compute_product(dims)};
    }
};

template <class T, index N, index D0, index... Ds>
struct tensor_reference<T, N, static_array<index, D0, Ds...>, true> {
    using type = tensor_base<T*, static_array<index, Ds...>>;

    template <class TensorPtr>
    static type get(TensorPtr ptr, index i) {
        return type{ptr->data() + i * static_product_v<Ds...>};
    }
};

template <class T, class Dims>
struct tensor_reference<T, 1, Dims, false> {
    using type = T&;

    template <class TensorPtr>
    static type get(TensorPtr ptr, index i) {
        return ptr->data()[i];
    }
};

template <class T, index N, class Dims>
using tensor_reference_t = typename tensor_reference<T, N, Dims>::type;

template <index N, class Data, class Dims>
class tensor_subscript : public tensor_shape<N, Data, Dims> {
    using base_t = tensor_shape<N, Data, Dims>;
    using T = value_type_t<base_t>;

public:
    using reference = tensor_reference_t<T, N, Dims>;
    using const_reference = tensor_reference_t<const T, N, Dims>;

    using base_t::base_t;

    constexpr auto dimension(index i) const {
        ACTL_ASSERT(0 <= i && i < this->rank());
        return this->dimensions()[static_cast<size_type_t<Dims>>(i)];
    }

    reference operator[](index i) {
        ACTL_ASSERT(0 <= i && i < dimension(0));
        return tensor_reference<T, N, Dims>::get(this, i);
    }

    const_reference operator[](index i) const {
        ACTL_ASSERT(0 <= i && i < dimension(0));
        return tensor_reference<const T, N, Dims>::get(this, i);
    }
};

template <class Data, class Dims>
class tensor_subscript<0, Data, Dims> : public tensor_shape<0, Data, static_array<index>> {
    using base_t = tensor_shape<0, Data, static_array<index>>;
    using T = value_type_t<base_t>;

public:
    using base_t::base_t;

    constexpr operator T&() {
        return *this->data();
    }
    constexpr operator const T&() const {
        return *this->data();
    }
};

/* Base class, defines type aliases and secondary interface methods. */

template <class Data, class Dims>
class tensor_base : public tensor_subscript<static_size_v<Dims>, Data, Dims> {
    using base_t = tensor_subscript<static_size_v<Dims>, Data, Dims>;
    using T = value_type_t<base_t>;

public:
    using size_type = index;
    using difference_type = index;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using base_t::base_t;

    iterator begin() {
        return this->data();
    }
    iterator end() {
        return begin() + this->size();
    }

    const_iterator begin() const {
        return this->data();
    }
    const_iterator end() const {
        return begin() + this->size();
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const {
        return begin();
    }
    const_iterator cend() const {
        return end();
    }

    const_reverse_iterator crbegin() const {
        return rbegin();
    }
    const_reverse_iterator crend() const {
        return rend();
    }

    bool empty() const {
        return this->size() == 0;
    }

    template <class... Ints>
    reference operator()(Ints... is) {
        return this->data()[get_index<0>(0, is...)];
    }

    template <class... Ints>
    const_reference operator()(Ints... is) const {
        return this->data()[get_index<0>(0, is...)];
    }

private:
    template <index I>
    index get_index(index res) const {
        ACTL_ASSERT(I == this->rank());
        return res;
    }

    template <index I, class... Ints>
    index get_index(index res, index i, Ints... is) const {
        ACTL_ASSERT(0 <= i && i < this->dimension(I));
        if constexpr (I > 0)
            res *= this->dimension(I);
        return get_index<I + 1>(res + i, is...);
    }
};

template <class D, class S>
void swap(tensor_base<D, S>& lhs, tensor_base<D, S>& rhs) {
    lhs.swap(rhs);
}

template <class T, size_t D, index... Dims>
struct tensor_fixed {
    using type = tensor_base<std::unique_ptr<T[]>, semi_static_array<int, Dims...>>;
};

template <class T, index... Dims>
struct tensor_fixed<T, 0, Dims...> {
    static constexpr index size = static_product_v<Dims...>;

    using type =
        tensor_base<std::conditional_t<16 < size, std::unique_ptr<T[]>, std::array<T, size>>,
                    static_array<index, Dims...>>;
};

template <index N>
struct dimensions {
    using type = std::array<int, N>;
};

template <>
struct dimensions<0> {
    using type = static_array<index>;
};

template <>
struct dimensions<dynamic_size> {
    using type = std::vector<int>;
};

template <index N>
using dimensions_t = typename dimensions<N>::type;

template <class T, index N>
struct tensor {
    using type = tensor_base<std::unique_ptr<T[]>, dimensions_t<N>>;
};

template <class T>
struct tensor<T, 0> : tensor_fixed<T, 0> {};

}  // namespace detail

struct tensor_tag {
    using base = unclassified_tag;
};

template <class... Ts>
struct category<ac::detail::tensor_base<Ts...>> {
    using type = tensor_tag;
};

struct tensor_equal_t {
    template <class EqualOp, class T, class U>
    static bool evaluate(const EqualOp& op, const T& lhs, const U& rhs) {
        if (lhs.rank() != rhs.rank())
            return false;
        for (index i = 0; i < lhs.rank(); ++i) {
            if (lhs.dimension(i) != rhs.dimension(i))
                return false;
        }
        return op(span{lhs}, span{rhs});
    }
};

template <class T, class U>
struct overload<equal_t, tensor_tag, T, U> {
    static constexpr auto resolve(equal_t op) {
        return operation_composer<tensor_equal_t>(op.resolve_nested<T, U>());
    }
};

/// N-dimensional array with dimensions completely or partially known at compile time.
template <class T, index... Dimensions>
using tensor_fixed =
    typename detail::tensor_fixed<T, (0 + ... + (Dimensions == dynamic_size)), Dimensions...>::type;

/// N-dimensional array.
template <class T, index N = dynamic_size>
using tensor = typename detail::tensor<T, N>::type;

/// View of an N-dimensional array.
template <class T, index N = dynamic_size>
using tensor_view = detail::tensor_base<T*, detail::dimensions_t<N>>;

}  // namespace ac
