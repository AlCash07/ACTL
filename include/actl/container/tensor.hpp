// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/container/array/semi_static_array.hpp>
#include <actl/meta/static_size.hpp>
#include <actl/operation/scalar/basic_math/max.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/range/operation/comparison.hpp>
#include <actl/range/span.hpp>
#include <actl/std/utility.hpp>
#include <actl/utility/index.hpp>
#include <memory>

namespace ac {

namespace detail {

template<typename Data, typename Dimensions>
class tensor_base;

template<typename T, size_t N>
struct nd_initializer_list {
    using type =
        std::initializer_list<typename nd_initializer_list<T, N - 1>::type>;
};

template<typename T>
struct nd_initializer_list<T, 0> {
    using type = T;
};

template<typename T>
struct nd_initializer_list<T, dynamic_size> {
    struct type {};
};

template<typename T, size_t N>
using nd_initializer_list_t = typename nd_initializer_list<T, N>::type;

template<size_t... Is>
inline constexpr size_t static_product_v = (1 * ... * Is);

template<typename Int>
size_t compute_product(cspan<Int> x) {
    size_t res = 1;
    for (auto v : x)
        res *= v;
    return res;
}

/* NDArray container class, supports array and std::unique_ptr as data. */

template<typename Data>
class tensor_container;

template<typename T, size_t Size>
class tensor_container<std::array<T, Size>> {
public:
    using value_type = T;

    tensor_container(size_t size) {
        AC_ASSERT(size == Size);
    }

    T* data() {
        return m_data.data();
    }

    T const* data() const {
        return m_data.data();
    }

    void swap(tensor_container& that) {
        std::swap(this->m_data, that.m_data);
    }

private:
    std::array<T, Size> m_data;
};

template<typename T>
class tensor_container<std::unique_ptr<T[]>> {
public:
    using value_type = T;

    tensor_container(size_t size) : m_data{new T[size]} {}

    T* data() {
        return m_data.get();
    }

    T const* data() const {
        return m_data.get();
    }

    void swap(tensor_container& that) {
        std::swap(this->m_data, that.m_data);
    }

private:
    std::unique_ptr<T[]> m_data;
};

/* NDArray data class, supports container or pointer as data. */

template<size_t N, typename Data>
class tensor_data : public tensor_container<Data> {
    using base_t = tensor_container<Data>;
    using T = typename base_t::value_type;

public:
    tensor_data(size_t size) : base_t{size} {}

    template<typename InRange>
    tensor_data(size_t size, InRange data) : base_t{size} {
        AC_ASSERT(size == ranges::size(data));
        std::copy_n(ranges::begin(data), size, this->data());
    }

    tensor_data(size_t size, T const& value) : base_t{size} {
        std::fill_n(this->data(), size, value);
    }

    template<typename Dims>
    tensor_data(size_t size, nd_initializer_list_t<T, N> const& il, Dims dims)
        : base_t{size} {
        // Array of size 0 is not standard-compliant.
        size_t strides[std::max(N - 1, size_t{1})];
        if constexpr (N >= 2) {
            strides[N - 2] = dims[N - 1];
            for (size_t i = N - 2; i > 0; --i)
                strides[i - 1] = strides[i] * dims[i - 1];
        }
        T* end = initialize<0>(this->data(), il, dims, strides);
        std::fill(end, this->data() + size, T{});
    }

    void swap(tensor_data& that) {
        base_t::swap(that);
    }

private:
    template<size_t I, typename Dims>
    T* initialize(
        T* ptr,
        nd_initializer_list_t<T, N - I> const& il,
        Dims dims,
        size_t const* strides
    ) {
        AC_ASSERT(il.size() <= static_cast<size_t>(dims[I]));
        if constexpr (I + 1 < N) {
            for (auto const& x : il) {
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

template<size_t N, typename T>
class tensor_data<N, T*> {
public:
    using value_type = T;

    tensor_data(size_t, T* ptr) : m_ptr{ptr} {}

    T* data() {
        return m_ptr;
    }

    T const* data() const {
        return m_ptr;
    }

    void swap(tensor_data& that) {
        std::swap(this->m_ptr, that.m_ptr);
    }

private:
    T* m_ptr;
};

/* NDArray shape class, manages dimensions. */

// This class is needed to guarantee dims construction before data to avoid
// undefined behavior.
template<typename Dims>
struct tensor_dims {
    Dims m_dims;
};

template<typename Int, size_t N>
struct tensor_dims<std::array<Int, N>> {
    tensor_dims() = default;

    tensor_dims(std::initializer_list<Int> dims) {
        std::copy_n(dims.begin(), N, m_dims.begin());
    }

    std::array<Int, N> m_dims = {};
};

template<size_t N, typename Data, typename Dims>
class tensor_shape
    : private tensor_dims<Dims>
    , public tensor_data<N, Data> {
    using base_dims = tensor_dims<Dims>;
    using base_data = tensor_data<N, Data>;
    using Int = range_value_t<Dims>;
    using T = typename base_data::value_type;

    using base_dims::m_dims;

public:
    template<typename... Ts>
    explicit tensor_shape(Dims const& dims, Ts... args)
        : base_dims{dims}, base_data{size(), args...} {}

    template<typename... Ts>
    explicit tensor_shape(Dims&& dims, Ts... args)
        : base_dims{std::move(dims)}, base_data{size(), args...} {}

    template<typename... Ts>
    explicit tensor_shape(
        std::conditional_t<N == 1, Int, std::initializer_list<Int>> dims,
        Ts... args
    )
        : base_dims{dims}, base_data{size(), args...} {}

    explicit tensor_shape(nd_initializer_list_t<T, N> il)
        : base_dims{}
        , base_data{
              (compute_dimensions<0>(il), size()), il, ranges::data(m_dims)
          } {}

    constexpr size_t rank() const {
        return ranges::size(m_dims);
    }

    size_t size() const {
        return compute_product<Int>(m_dims);
    }

    Dims const& dimensions() const {
        return m_dims;
    }

    void swap(tensor_shape& that) {
        using std::swap;
        swap(this->m_dims, that.m_dims);
        base_data::swap(that);
    }

private:
    template<size_t I>
    void compute_dimensions(nd_initializer_list_t<T, N - I> il) {
        if constexpr (I < N) {
            max(inout{m_dims[I]}, static_cast<Int>(il.size()));
            for (auto i : il)
                compute_dimensions<I + 1>(i);
        }
    }
};

template<size_t N, typename Data, size_t... Ds>
class tensor_shape<N, Data, static_array<size_t, Ds...>>
    : public tensor_data<N, Data> {
    using base_t = tensor_data<N, Data>;
    using m_dimst = static_array<size_t, Ds...>;

public:
    template<typename... Ts>
    explicit tensor_shape(Ts... args) : base_t{size(), args...} {}

    template<bool B = 0 < N>
        requires B
    explicit tensor_shape(
        nd_initializer_list_t<typename base_t::value_type, N> il
    )
        : base_t{size(), il, dimensions()} {}

    static constexpr size_t rank() {
        return static_size_v<m_dimst>;
    }

    static constexpr size_t size() {
        return static_product_v<Ds...>;
    }

    static constexpr m_dimst dimensions() {
        return {};
    }
};

/* NDArray subscript operator implementation */

template<typename T, size_t N, typename Dims, bool = 1 < N>
struct tensor_reference {
    using Int = range_value_t<Dims>;
    using type = tensor_base<T*, span<Int, N - 1>>;

    template<typename TensorPtr>
    static type get(TensorPtr ptr, size_t i) {
        span<Int, N - 1> dims{ranges::data(ptr->dimensions()) + 1, N - 1};
        return type{dims, ptr->data() + i * compute_product(dims)};
    }
};

template<typename T, size_t N, size_t D0, size_t... Ds>
struct tensor_reference<T, N, static_array<size_t, D0, Ds...>, true> {
    using type = tensor_base<T*, static_array<size_t, Ds...>>;

    template<typename TensorPtr>
    static type get(TensorPtr ptr, size_t i) {
        return type{ptr->data() + i * static_product_v<Ds...>};
    }
};

template<typename T, typename Dims>
struct tensor_reference<T, 1, Dims, false> {
    using type = T&;

    template<typename TensorPtr>
    static type get(TensorPtr ptr, size_t i) {
        return ptr->data()[i];
    }
};

template<typename T, size_t N, typename Dims>
using tensor_reference_t = typename tensor_reference<T, N, Dims>::type;

template<size_t N, typename Data, typename Dims>
class tensor_subscript : public tensor_shape<N, Data, Dims> {
    using base_t = tensor_shape<N, Data, Dims>;
    using T = typename base_t::value_type;

public:
    using reference = tensor_reference_t<T, N, Dims>;
    using const_reference = tensor_reference_t<T const, N, Dims>;

    using base_t::base_t;

    constexpr auto dimension(size_t i) const {
        AC_ASSERT(0 <= i && i < this->rank());
        return this->dimensions()[static_cast<range_size_t<Dims>>(i)];
    }

    reference operator[](size_t i) {
        AC_ASSERT(0 <= i && i < dimension(0));
        return tensor_reference<T, N, Dims>::get(this, i);
    }

    const_reference operator[](size_t i) const {
        AC_ASSERT(0 <= i && i < dimension(0));
        return tensor_reference<T const, N, Dims>::get(this, i);
    }
};

template<typename Data, typename Dims>
class tensor_subscript<0, Data, Dims>
    : public tensor_shape<0, Data, static_array<size_t>> {
    using base_t = tensor_shape<0, Data, static_array<size_t>>;
    using T = typename base_t::value_type;

public:
    using base_t::base_t;

    constexpr operator T&() {
        return *this->data();
    }

    constexpr operator T const&() const {
        return *this->data();
    }
};

/* Base class, defines type aliases and secondary interface methods. */

template<typename Data, typename Dims>
class tensor_base : public tensor_subscript<static_size_v<Dims>, Data, Dims> {
    using base_t = tensor_subscript<static_size_v<Dims>, Data, Dims>;
    using T = typename base_t::value_type;

public:
    using size_type = size_t;
    using difference_type = index;
    using reference = T&;
    using const_reference = T const&;
    using pointer = T*;
    using const_pointer = T const*;
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

    template<typename... Ints>
    reference operator()(Ints... is) {
        return this->data()[get_index<0>(0, static_cast<size_t>(is)...)];
    }

    template<typename... Ints>
    const_reference operator()(Ints... is) const {
        return this->data()[get_index<0>(0, static_cast<size_t>(is)...)];
    }

private:
    template<size_t I>
    size_t get_index(size_t res) const {
        AC_ASSERT(I == this->rank());
        return res;
    }

    template<size_t I, typename... Ints>
    size_t get_index(size_t res, size_t i, Ints... is) const {
        AC_ASSERT(0 <= i && i < this->dimension(I));
        if constexpr (I > 0)
            res *= this->dimension(I);
        return get_index<I + 1>(res + i, is...);
    }
};

template<typename D, typename S>
void swap(tensor_base<D, S>& l, tensor_base<D, S>& r) {
    l.swap(r);
}

template<typename T, size_t D, size_t... Dims>
struct tensor_fixed {
    using type =
        tensor_base<std::unique_ptr<T[]>, semi_static_array<int, Dims...>>;
};

template<typename T, size_t... Dims>
struct tensor_fixed<T, 0, Dims...> {
    static constexpr size_t size = static_product_v<Dims...>;

    using type = tensor_base<
        std::
            conditional_t<16 < size, std::unique_ptr<T[]>, std::array<T, size>>,
        static_array<size_t, Dims...>>;
};

template<size_t N>
struct dimensions {
    using type = std::array<size_t, N>;
};

template<>
struct dimensions<0> {
    using type = static_array<size_t>;
};

template<>
struct dimensions<dynamic_size> {
    using type = std::vector<size_t>;
};

template<size_t N>
using dimensions_t = typename dimensions<N>::type;

template<typename T, size_t N>
struct tensor {
    using type = tensor_base<std::unique_ptr<T[]>, dimensions_t<N>>;
};

template<typename T>
struct tensor<T, 0> : tensor_fixed<T, 0> {};

template<typename T>
struct is_tensor : std::false_type {};

template<typename... Ts>
struct is_tensor<ac::detail::tensor_base<Ts...>> : std::true_type {};

} // namespace detail

template<typename T>
concept Tensor = detail::is_tensor<T>::value;

struct tensor_equal_f {
    static constexpr size_t inner_count = 1;

    template<typename EqualOp, typename L, typename R>
    static bool evaluate(EqualOp const& op, L const& l, R const& r) {
        if (l.rank() != r.rank())
            return false;
        for (size_t i = 0; i < l.rank(); ++i) {
            if (l.dimension(i) != r.dimension(i))
                return false;
        }
        return op(span{l}, span{r});
    }
};

template<Tensor T, Tensor U>
struct overload<equal_f, T, U> {
    static constexpr auto formula =
        operation_composer<tensor_equal_f>(resolve_nested<T, U>(equal));
};

/// N-dimensional array with dimensions completely or partially known at compile
/// time.
template<typename T, size_t... Dimensions>
using tensor_fixed = typename detail::tensor_fixed<
    T,
    (0 + ... + (Dimensions == dynamic_size)),
    Dimensions...>::type;

/// N-dimensional array.
template<typename T, size_t N = dynamic_size>
using tensor = typename detail::tensor<T, N>::type;

/// View of an N-dimensional array.
template<typename T, size_t N = dynamic_size>
using tensor_view = detail::tensor_base<T*, detail::dimensions_t<N>>;

} // namespace ac
