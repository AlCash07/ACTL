/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/functions.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/traits/type_traits.hpp>
#include <array>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

namespace ac {

namespace detail {

template <int N, class Data, class Dimensions>
class ndarray_base;

/* Helper types. */

template <int...>
struct static_array {
    int operator[](int i) const {
        ACTL_ASSERT(false);
        return -1;
    }
};

template <int I0, int... Is>
struct static_array<I0, Is...> {
    constexpr int operator[](int i) const {
        return i == 0 ? I0 : static_array<Is...>()[i - 1];
    }
};

template <int...> struct strides;

template <int... Ds>
using strides_t = typename strides<Ds...>::type;

template <int D>
struct strides<D> { using type = static_array<D>; };

template <int D0, int I0, int... Is>
static_array<D0 * I0, I0, Is...> strides_impl(static_array<I0, Is...>);

template <int D0, int D1, int... Ds>
struct strides<D0, D1, Ds...> {
    using type = decltype(strides_impl<D0>(strides_t<D1, Ds...>()));
};

template <class T, int N>
struct nd_initializer_list {
    using type = std::initializer_list<typename nd_initializer_list<T, N - 1>::type>;
};

template <class T>
struct nd_initializer_list<T, 0> { using type = T; };

template <class T, int N>
using nd_initializer_list_t = typename nd_initializer_list<T, N>::type;

/* NDArray container class, supports std::array and std::unique_ptr as data. */

template <class Data>
class ndarray_container;

template <class T, int Size>
class ndarray_container<std::array<T, Size>> {
public:
    using value_type = T;

    ndarray_container(int size) {}

    T*       data() { return data_.data(); }
    const T* data() const { return data_.data(); }

    void swap(ndarray_container& other) { std::swap(data_, other.data_); }

private:
    std::array<T, Size> data_;
};

template <class T>
class ndarray_container<std::unique_ptr<T>> {
public:
    using value_type = T;

    ndarray_container(int size) : data_(new T[size]) {}

    T*       data() { return data_.get(); }
    const T* data() const { return data_.get(); }

    void swap(ndarray_container& other) { std::swap(data_, other.data_); }

private:
    std::unique_ptr<T> data_;
};

/* NDArray data class, supports container or pointer as data. */

template <int N, class Data>
class ndarray_data : public ndarray_container<Data> {
    using base_t = ndarray_container<Data>;
    using T      = typename base_t::value_type;

public:
    ndarray_data(int size) : base_t(size) {}

    template <class InputIterator>
    ndarray_data(int size, InputIterator it) : base_t(size) {
        std::copy_n(it, size, this->data());
    }

    template <class Strides>
    explicit ndarray_data(int size, nd_initializer_list_t<T, N> ilist, Strides strides)
        : base_t(size) {
        std::fill_n(this->data(), size, T{});
        initialize<0>(this->data(), ilist, strides);
    }

    void swap(ndarray_data& other) { base_t::swap(other); }

private:
    template <int I, class Strides>
    std::enable_if_t<I + 1 < N> initialize(T* data, nd_initializer_list_t<T, N - I> ilist,
                                           Strides strides) {
        ACTL_ASSERT(static_cast<int>(ilist.size()) * strides[I + 1] <= strides[I]);
        for (auto it : ilist) {
            initialize<I + 1>(data, it, strides);
            data += strides[I + 1];
        }
    }

    template <int I, class Strides>
    std::enable_if_t<I + 1 == N> initialize(T* data, std::initializer_list<T> ilist,
                                            Strides strides) {
        ACTL_ASSERT(static_cast<int>(ilist.size()) <= strides[I]);
        copy(ilist, data);
    }
};

template <int N, class T>
class ndarray_data<N, T*> {
public:
    using value_type = T;

    ndarray_data(int, T* data) : data_(data) {}

    void swap(ndarray_data& other) { std::swap(data_, other.data_); }

    T*       data() { return data_; }
    const T* data() const { return data_; }

private:
    T* data_;
};

/* NDArray shape class, manages dimensions, strides and subscript operator. */

template <int N, class Data, class Dims>
class ndarray_shape : public ndarray_data<N, Data> {
    using base_t = ndarray_data<N, Data>;
    using T      = typename base_t::value_type;

public:
    template <class... Ts>
    explicit ndarray_shape(const Dims& dims, Ts... args) : base_t(compute_size(dims), args...) {}

    template <class... Ts>
    explicit ndarray_shape(int dimension, Ts... args)
        : ndarray_shape(std::make_tuple(dimension), args...) {}

    explicit ndarray_shape(nd_initializer_list_t<T, N> ilist)
        : base_t(compute_size(ilist), ilist, strides()) {}

    void swap(ndarray_shape& other) {
        base_t::swap(other);
        std::swap(strides_, other.strides_);
    }

    const int* strides() const { return strides_.data(); }

private:
    template <class... Ts, int... Is>
    static Dims tuple_to_array(std::tuple<Ts...> dims, std::integer_sequence<int, Is...>) {
        static_assert(sizeof...(Ts) == N, "wrong number of dimensions given");
        return Dims{{std::get<Is>(dims)...}};
    }

    template <class... Us, class... Ts>
    ndarray_shape(std::tuple<Us...> dims, Ts... args)
        : ndarray_shape(tuple_to_array(dims, std::make_integer_sequence<int, N>()), args...) {}

    template <class... Us, class... Ts>
    ndarray_shape(std::tuple<Us...> dims, int dimension, Ts... args)
        : ndarray_shape(std::tuple_cat(dims, std::make_tuple(dimension)), args...) {}

    int compute_size(const Dims& dims) {
        int size = 1;
        for (int i = N - 1; i >= 0; --i)
            strides_[i] = size *= dims[i];
        return size;
    }

    int compute_size(nd_initializer_list_t<T, N> ilist) {
        strides_.fill(0);
        compute_dimensions<0>(ilist);
        return compute_size(strides_);
    }

    template <int I>
    std::enable_if_t<I < N> compute_dimensions(nd_initializer_list_t<T, N - I> ilist) {
        smax(strides_[I], static_cast<int>(ilist.size()));
        for (auto it : ilist) compute_dimensions<I + 1>(it);
    }

    template <int I>
    std::enable_if_t<I == N> compute_dimensions(nd_initializer_list_t<T, 0>) {}

    Dims strides_;
};

template <int N, class T>
class ndarray_shape<N, T*, const int*> : public ndarray_data<N, T*> {
    using base_t = ndarray_data<N, T*>;

public:
    explicit ndarray_shape(const int* strides, T* data)
        : base_t(strides[0], data), strides_(strides) {}

    void swap(ndarray_shape& other) {
        base_t::swap(other);
        std::swap(strides_, other.strides_);
    }

    const int* strides() const { return strides_; }

private:
    const int* strides_;
};

template <int N, class Data, int... Ds>
class ndarray_shape<N, Data, static_array<Ds...>> : public ndarray_data<N, Data> {
    using base_t = ndarray_data<N, Data>;

public:
    template <class... Ts>
    explicit ndarray_shape(Ts... args) : base_t(0, args...) {}

    explicit ndarray_shape(nd_initializer_list_t<typename base_t::value_type, N> ilist)
        : base_t(0, ilist, strides()) {}

    static constexpr strides_t<Ds...> strides() { return {}; }
};

template <class Data>
class ndarray_shape<0, Data, static_array<>> : public ndarray_data<0, Data> {
    using base_t = ndarray_data<0, Data>;
    using T      = typename base_t::value_type;

public:
    template <class... Ts>
    explicit ndarray_shape(Ts... args) : base_t(1, args...) {}

    explicit ndarray_shape(T value) : base_t(1) { *this->data() = value; }

    constexpr operator T&() { return *this->data(); }
    constexpr operator const T&() const { return *this->data(); }
};

/* NDArray subscript operator implementation */

template <class T, int N, class D, bool = 1 < N>
struct ndarray_reference {
    using type = ndarray_base<N - 1, T*, const int*>;

    template <class NDArrayPtr>
    static type get(NDArrayPtr ptr, int i) {
        ACTL_ASSERT(0 <= i && i < ptr->dimension(0));
        return type(ptr->strides() + 1, ptr->data() + i * ptr->stride(1));
    }
};

template <class T, int N, int D0, int... Ds>
struct ndarray_reference<T, N, static_array<D0, Ds...>, true> {
    using type = ndarray_base<N - 1, T*, static_array<Ds...>>;

    template <class NDArrayPtr>
    static type get(NDArrayPtr ptr, int i) {
        ACTL_ASSERT(0 <= i && i < ptr->dimension(0));
        return type(ptr->data() + i * ptr->stride(1));
    }
};

template <class T, class D>
struct ndarray_reference<T, 1, D, false> {
    using type = T&;

    template <class NDArrayPtr>
    static type get(NDArrayPtr ptr, int i) {
        ACTL_ASSERT(0 <= i && i < ptr->dimension(0));
        return ptr->data()[i];
    }
};

template <class T, int N, class D>
using ndarray_reference_t = typename ndarray_reference<T, N, D>::type;

template <int N, class Data, class Dims>
class ndarray_subscript : public ndarray_shape<N, Data, Dims> {
    using base_t = ndarray_shape<N, Data, Dims>;
    using T      = typename base_t::value_type;

public:
    using reference       = ndarray_reference_t<T, N, Dims>;
    using const_reference = ndarray_reference_t<const T, N, Dims>;

    using base_t::base_t;

    constexpr int size() const { return stride(0); }

    constexpr int dimension(int i) const {
        ACTL_ASSERT(0 <= i && i < N);
        return stride(i) / (i + 1 < N ? stride(i + 1) : 1);
    }

    constexpr int stride(int i) const {
        ACTL_ASSERT(0 <= i && i < N);
        return this->strides()[i];
    }

    reference operator[](int i) {
        return ndarray_reference<T, N, Dims>::get(this, i);
    }

    const_reference operator[](int i) const {
        return ndarray_reference<const T, N, Dims>::get(this, i);
    }
};

template <class Data, class Dims>
class ndarray_subscript<0, Data, Dims> : public ndarray_shape<0, Data, static_array<>> {
public:
    using ndarray_shape<0, Data, static_array<>>::ndarray_shape;

    static constexpr int size() { return 1; }
};

/* Base class, defines type aliases and secondary interface methods. */

template <int N, class Data, class Dims = std::array<int, N>>
class ndarray_base : public ndarray_subscript<N, Data, Dims> {
    using base_t = ndarray_subscript<N, Data, Dims>;
    using T      = typename base_t::value_type;

public:
    using size_type              = int;
    using difference_type        = int;
    using pointer                = T*;
    using const_pointer          = const T*;
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr const int rank() { return N; }

    using base_t::base_t;

    void fill(const T& value) { std::fill_n(this->data(), this->size(), value); }

    void clear() { fill(T{}); }

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
    T& operator()(Ints... indices) {
        return this->data()[getIndex<0>(indices...)];
    }

    template <class... Ints>
    const T& operator()(Ints... indices) const {
        return this->data()[getIndex<0>(indices...)];
    }

private:
    template <int I>
    int getIndex() const {
        static_assert(I == N, "number of indices isn't equal to ndarray rank");
        return 0;
    }

    template <int I, class... Ints>
    int getIndex(int i, Ints... indices) const {
        ACTL_ASSERT(0 <= i && i < this->dimension(I));
        return (I + 1 < N ? i * this->stride(I + 1) : i) + getIndex<I + 1>(indices...);
    }
};

template <int N0, int N1>
struct strides_compare {
    template <class T0, class T1>
    static bool equal(const T0&, const T1&) { return false; }
};

template <int N>
struct strides_compare<N, N> {
    template <class T0, class T1>
    static bool equal(const T0& lhs, const T1& rhs) {
        for (int i = 0; i < N; ++i)
            if (lhs.stride(i) != rhs.stride(i)) return false;
        return true;
    }
};

template <>
struct strides_compare<0, 0> {
    template <class T0, class T1>
    static bool equal(const T0&, const T1&) { return true; }
};

template <int N0, class D0, class S0, int N1, class D1, class S1>
inline bool operator == (const ndarray_base<N0, D0, S0>& lhs, const ndarray_base<N1, D1, S1>& rhs) {
    return strides_compare<N0, N1>::equal(lhs, rhs) &&
           std::equal(lhs.data(), lhs.data() + lhs.size(), rhs.data());
}

template <int N0, class D0, class S0, int N1, class D1, class S1>
inline bool operator != (const ndarray_base<N0, D0, S0>& lhs, const ndarray_base<N1, D1, S1>& rhs) {
    return !(lhs == rhs);
}

template <int N, class D, class S>
inline void swap(ndarray_base<N, D, S>& lhs, ndarray_base<N, D, S>& rhs) { lhs.swap(rhs); }

template <int...>
struct ndarray_size : int_constant<1> {};

template <int D0, int... Ds>
struct ndarray_size<D0, Ds...> : int_constant<D0 * ndarray_size<Ds...>::value> {};

template <class T, int... Ds>
using ndarray_base_static =
    ndarray_base<sizeof...(Ds), std::array<T, ndarray_size<Ds...>::value>, static_array<Ds...>>;

}  // namespace detail

/**
 * N-dimensional array.
 */
template <class T, int N>
class ndarray : public detail::ndarray_base<N, std::unique_ptr<T>> {
    using base_t = detail::ndarray_base<N, std::unique_ptr<T>>;

public:
    using base_t::base_t;
};

/**
 * N-dimensional array with fixed dimensions.
 */
template <class T, int... Dimensions>
class ndarray_static : public detail::ndarray_base_static<T, Dimensions...> {
    using base_t = detail::ndarray_base_static<T, Dimensions...>;

public:
    using base_t::base_t;
};

/**
 * View of an N-dimensional array.
 */
template <class T, int N>
class ndarray_view : public detail::ndarray_base<N, T*> {
    using base_t = detail::ndarray_base<N, T*>;

public:
    using base_t::base_t;

    template <class Data, class Dimensions>
    ndarray_view& operator = (const detail::ndarray_base<N, Data, Dimensions>& other) {
        ACTL_ASSERT(this->size() == other.size());
        copy(*this, other.begin());
        return *this;
    }
};

}  // namespace ac
