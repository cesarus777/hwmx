// -------------------------------------------------------------------------- //
// Copyright 2022 Yuly Tarasov
//
// This file is part of hwmx.
//
// hwmx is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// hwmx is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// hwmx. If not, see <https://www.gnu.org/licenses/>.
// -------------------------------------------------------------------------- //

#pragma once

#include "Concepts.hpp"
#include "FixedVector.hpp"

#include <functional>

namespace mmm { // my magic matrix

template <Arithmetic T> class slice_iterator;
template <Arithmetic T> class const_slice_iterator;

template <Arithmetic T> class Slice {
public:
  using Elem = T;
  using Data = Elem *;

protected:
  Data data_;
  size_t start_;
  size_t stride_;
  size_t n_;

  size_t getIdx(size_t i) const noexcept { return start_ + (i % n_) * stride_; }

public:
  Slice(Data data, size_t data_size, size_t start, size_t stride, size_t n)
      : data_(data), start_(start), stride_(stride), n_(n) {
    size_t last_id = start_ + (n_ - 1) * stride_;
    if (n_ == 0)
      throw std::out_of_range("Slice can't be empty");

    if (last_id >= data_size)
      throw std::out_of_range(
          "Slice size is more than size of underlying data");
  }

  template <std::ranges::random_access_range R>
  Slice(R &&r) : data_(r.begin()), start_(0), stride_(1), n_(r.size()) {}

  template <std::ranges::random_access_range R>
  Slice(R &&r, size_t start, size_t stride, size_t n)
      : data_(r.begin()), start_(start), stride_(stride), n_(n) {
    size_t last_id = start_ + (n_ - 1) * stride_;
    if (n_ == 0)
      throw std::out_of_range("Slice can't be empty");

    if (last_id >= std::ranges::size(r))
      throw std::out_of_range(
          "Slice size is more than size of underlying data");
  }

  Slice &operator=(const Elem &elem) { std::ranges::fill(*this, elem); }

  Slice &operator=(const FixedVector<T> &vec) {
    auto n = std::min(n_, vec.size());
    std::ranges::copy(vec | std::views::take(n), *this);
  }

  Slice &operator=(FixedVector<T> &&vec) {
    auto n = std::min(n_, vec.size());
    std::ranges::move(vec | std::views::take(n), *this);
  }

  [[nodiscard]] size_t size() const noexcept { return n_; }

  auto begin() noexcept { return slice_iterator(data_ + start_, stride_); }
  auto end() noexcept {
    return slice_iterator(data_ + start_ + n_ * stride_, stride_);
  }

  auto begin() const noexcept {
    return const_slice_iterator<Elem>(data_ + start_, stride_);
  }
  auto end() const noexcept {
    return const_slice_iterator<Elem>(data_ + start_ + n_ * stride_, stride_);
  }

  [[nodiscard]] T operator[](size_t i) const { return data_[getIdx(i)]; }

  [[nodiscard]] T &operator[](size_t i) { return data_[getIdx(i)]; }

  auto &operator+=(const Slice &other) {
    std::ranges::transform(*this, other, begin(), std::plus{});
    return *this;
  }

  auto &operator+=(const FixedVector<T> &other) {
    if (other.size() != n_)
      throw std::runtime_error(
          "Can't add Slice and FixedVector with different sizes");

    std::ranges::transform(*this, other, begin(), std::plus{});
    return *this;
  }

  auto &operator+=(const T &other) {
    std::ranges::for_each(*this, [other](T &elem) { elem += other; });
    return *this;
  }

  auto &operator-=(const Slice &other) {
    std::ranges::transform(*this, other, begin(), std::minus{});
    return *this;
  }

  auto &operator-=(const FixedVector<T> &other) {
    if (other.size() != n_)
      throw std::runtime_error(
          "Can't sub Slice and FixedVector with different sizes");

    std::ranges::transform(*this, other, begin(), std::minus{});
    return *this;
  }

  auto &operator-=(const T &other) {
    std::ranges::for_each(*this, [other](T &elem) { elem -= other; });
    return *this;
  }

  auto &operator*=(const Slice &other) {
    std::ranges::transform(*this, other, begin(), std::multiplies{});
    return *this;
  }

  auto &operator*=(const FixedVector<T> &other) {
    if (other.size() != n_)
      throw std::runtime_error(
          "Can't mul Slice and FixedVector with different sizes");

    std::ranges::transform(*this, other, begin(), std::multiplies{});
    return *this;
  }

  auto &operator*=(const T &other) {
    std::ranges::for_each(*this | std::views::all,
                          [other](T &elem) { elem *= other; });
    return *this;
  }

  auto &operator/=(const Slice &other) {
    std::ranges::transform(*this, other, begin(), std::divides{});
    return *this;
  }

  auto &operator/=(const FixedVector<T> &other) {
    if (other.size() != n_)
      throw std::runtime_error(
          "Can't div Slice and FixedVector with different sizes");

    std::ranges::transform(*this, other, begin(), std::divides{});
    return *this;
  }

  auto &operator/=(const T &other) {
    std::ranges::for_each(*this, [other](T &elem) { elem /= other; });
    return *this;
  }
}; // class Slice

template <Arithmetic T> class slice_iterator {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using reference = T &;
  using pointer = T *;
  using difference_type = std::ptrdiff_t;

  friend const_slice_iterator<T>;

private:
  pointer ptr_ = nullptr;
  difference_type stride_ = 0;

public:
  constexpr slice_iterator() = default;
  constexpr slice_iterator(pointer ptr, difference_type stride) noexcept
      : ptr_(ptr), stride_(stride) {}

  constexpr slice_iterator &operator++() noexcept {
    ptr_ += stride_;
    return *this;
  }

  constexpr slice_iterator operator++(int) noexcept {
    slice_iterator ret = *this;
    ++(*this);
    return ret;
  }

  constexpr slice_iterator &operator--() noexcept {
    ptr_ -= stride_;
    return *this;
  }

  constexpr slice_iterator operator--(int) const noexcept {
    slice_iterator ret = *this;
    --(*this);
    return ret;
  }

  constexpr difference_type
  operator-(const slice_iterator &other) const noexcept {
    return (ptr_ - other.ptr_) / stride_;
  }

  constexpr slice_iterator &operator+=(difference_type n) noexcept {
    ptr_ += n * stride_;
    return *this;
  }

  constexpr slice_iterator &operator-=(difference_type n) noexcept {
    ptr_ -= n * stride_;
    return *this;
  }

  constexpr slice_iterator operator+(difference_type n) const noexcept {
    slice_iterator ret = *this;
    ret += n;
    return ret;
  }

  constexpr slice_iterator operator-(difference_type n) const noexcept {
    slice_iterator ret = *this;
    ret -= n;
    return ret;
  }

  constexpr reference operator[](difference_type n) const noexcept {
    return *(ptr_ + n * stride_);
  }

  constexpr reference operator[](difference_type n) noexcept {
    return *(ptr_ + n * stride_);
  }

  constexpr std::strong_ordering
  operator<=>(const slice_iterator &other) const noexcept {
    return ptr_ <=> other.ptr_;
  }

  constexpr bool
  operator==(const slice_iterator &other) const noexcept = default;
  constexpr bool
  operator!=(const slice_iterator &other) const noexcept = default;
  constexpr bool
  operator<(const slice_iterator &other) const noexcept = default;
  constexpr bool
  operator<=(const slice_iterator &other) const noexcept = default;
  constexpr bool
  operator>(const slice_iterator &other) const noexcept = default;
  constexpr bool
  operator>=(const slice_iterator &other) const noexcept = default;

  constexpr reference operator*() const noexcept { return *ptr_; }
  constexpr pointer operator->() const noexcept { return ptr_; }
}; // class slice_iterator

template <Arithmetic T>
constexpr auto operator+(const typename slice_iterator<T>::difference_type n,
                         const slice_iterator<T> &j) {
  auto ret = j;
  ret += n;
  return ret;
}

template <Arithmetic T> class const_slice_iterator {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = const T;
  using reference = const T &;
  using pointer = const T *;
  using difference_type = std::ptrdiff_t;

private:
  pointer ptr_ = nullptr;
  difference_type stride_ = 0;

public:
  constexpr const_slice_iterator() = default;

  constexpr const_slice_iterator(pointer ptr, difference_type stride) noexcept
      : ptr_(ptr), stride_(stride) {}

  constexpr const_slice_iterator(const slice_iterator<T> &i) noexcept
      : ptr_(i.ptr_), stride_(i.stride_) {}

  constexpr const_slice_iterator &
  operator=(const slice_iterator<T> &i) noexcept {
    ptr_ = i.ptr_;
    stride_ = i.stride_;
  }

  constexpr const_slice_iterator(slice_iterator<T> &&i) noexcept
      : ptr_(i.ptr_), stride_(i.stride_) {}

  constexpr const_slice_iterator &operator=(slice_iterator<T> &&i) noexcept {
    ptr_ = i.ptr_;
    stride_ = i.stride_;
  }

  constexpr const_slice_iterator &operator++() {
    ptr_ += stride_;
    return *this;
  }

  constexpr const_slice_iterator operator++(int) noexcept {
    const_slice_iterator ret = *this;
    ++(*this);
    return ret;
  }

  constexpr const_slice_iterator &operator--() noexcept {
    ptr_ -= stride_;
    return *this;
  }

  constexpr const_slice_iterator operator--(int) noexcept {
    const_slice_iterator ret = *this;
    --(*this);
    return ret;
  }

  constexpr difference_type
  operator-(const const_slice_iterator &other) const noexcept {
    return (ptr_ - other.ptr_) / stride_;
  }

  constexpr const_slice_iterator &operator+=(difference_type n) noexcept {
    ptr_ += n * stride_;
    return *this;
  }

  constexpr const_slice_iterator &operator-=(difference_type n) noexcept {
    ptr_ -= n * stride_;
    return *this;
  }

  constexpr const_slice_iterator operator+(difference_type n) const noexcept {
    const_slice_iterator ret = *this;
    ret += n;
    return ret;
  }

  constexpr const_slice_iterator operator-(difference_type n) const noexcept {
    const_slice_iterator ret = *this;
    ret -= n;
    return ret;
  }

  constexpr reference operator[](difference_type n) const noexcept {
    return *(ptr_ + n * stride_);
  }

  constexpr reference operator[](difference_type n) noexcept {
    return *(ptr_ + n * stride_);
  }

  constexpr std::strong_ordering
  operator<=>(const const_slice_iterator &other) const noexcept {
    return ptr_ <=> other.ptr_;
  }

  constexpr bool
  operator==(const const_slice_iterator &other) const noexcept = default;
  constexpr bool
  operator!=(const const_slice_iterator &other) const noexcept = default;
  constexpr bool
  operator<(const const_slice_iterator &other) const noexcept = default;
  constexpr bool
  operator<=(const const_slice_iterator &other) const noexcept = default;
  constexpr bool
  operator>(const const_slice_iterator &other) const noexcept = default;
  constexpr bool
  operator>=(const const_slice_iterator &other) const noexcept = default;

  constexpr reference operator*() const noexcept { return *ptr_; }
  constexpr pointer operator->() const noexcept { return ptr_; }
}; // class const_slice_iterator

template <Arithmetic T>
constexpr auto
operator+(const typename const_slice_iterator<T>::difference_type n,
          const const_slice_iterator<T> &j) {
  auto ret = j;
  ret += n;
  return ret;
}

template <Arithmetic T> auto operator+(const T &lhs, const Slice<T> &rhs) {
  FixedVector<T> data(rhs);
  Slice<T>(data) += lhs;
  return data;
}

template <Arithmetic T> auto operator-(const T &lhs, const Slice<T> &rhs) {
  FixedVector<T> data(rhs.size());
  Slice<T> lhs_slice(data);
  lhs_slice = lhs;
  lhs_slice -= rhs;
  return data;
}

template <Arithmetic T> auto operator*(const T &lhs, const Slice<T> &rhs) {
  FixedVector<T> data(rhs);
  Slice<T>(data) *= lhs;
  return data;
}

template <Arithmetic T> auto operator/(const T &lhs, const Slice<T> &rhs) {
  FixedVector<T> data(rhs.size());
  Slice<T> lhs_slice(data);
  lhs_slice = lhs;
  lhs_slice /= rhs;
  return data;
}

} // namespace mmm
