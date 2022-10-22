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

#include <algorithm>
#include <concepts>
#include <ranges>
#include <stdexcept>
#include <string>

namespace mmm { // my magic matrix

// TODO: if T is not primitive and has heavy ctor, FixedVector ctors with copies
// to data_ can be unefficient
template <std::copyable T> class FixedVector {
public:
  using Elem = T;
  using ElemPtr = T *;
  using Data = ElemPtr;

private:
  T *data_;
  size_t size_;

public:
  constexpr FixedVector(size_t size = 0) : data_(nullptr), size_(size) {
    data_ = new T[size_];
    size_ = size;
  }

  template <std::input_iterator I>
  FixedVector(I i, size_t n) : data_(nullptr), size_(n) {
    data_ = new T[size_];
    std::ranges::copy_n(i, size_, data_);
  }

  template <std::ranges::input_range R>
  FixedVector(R &&r, size_t n) : data_(nullptr), size_(n) {
    data_ = new T[size_];
    std::ranges::copy(r | std::views::take(size_), data_);
  }

  template <std::random_access_iterator I, std::sentinel_for<I> S>
  FixedVector(I i, S s) : data_(nullptr), size_(s - i) {
    data_ = new T[size_];
    std::ranges::copy(i, s, data_);
  }

  template <std::ranges::random_access_range R>
  FixedVector(R &&r) : data_(nullptr), size_(std::ranges::size(r)) {
    data_ = new T[size_];
    std::ranges::copy(r | std::views::take(size_), data_);
  }

  template <std::convertible_to<Elem> U>
  FixedVector(const FixedVector<U> &other) {
    size_ = other.size();
    data_ = new T[size_];
    std::ranges::copy(other, data_);
  }

  FixedVector(const FixedVector &other) {
    size_ = other.size();
    data_ = new T[size_];
    std::ranges::copy(other, data_);
  }

  FixedVector &operator=(const FixedVector &other) {
    size_ = other.size_;
    delete[] data_;
    data_ = new T[size_];
    std::ranges::copy(other, data_);
  }

  FixedVector &operator=(FixedVector &&other) noexcept {
    data_ = other.data_;
    size_ = other.size_;
    other.data_ = nullptr;
    other.size = 0;
  }

  FixedVector(FixedVector &&other) noexcept {
    data_ = other.data_;
    size_ = other.size_;
    other.data_ = nullptr;
    other.size_ = 0;
  }

  ~FixedVector() noexcept {
    if (data_)
      delete[] data_;
  }

  [[nodiscard]] size_t size() const noexcept { return size_; }

  [[nodiscard]] T *begin() noexcept { return data_; }
  [[nodiscard]] T *end() noexcept { return data_ + size_; }
  [[nodiscard]] T *begin() const noexcept { return data_; }
  [[nodiscard]] T *end() const noexcept { return data_ + size_; }

  void checkBound(size_t i) const {
    if (i >= size_)
      throw std::out_of_range(std::string("Too big index ")
                                  .append(std::to_string(i))
                                  .append(" for mmm::FixedVector size of ")
                                  .append(std::to_string(size_)));
  }

  [[nodiscard]] T operator[](size_t i) const {
    checkBound(i);
    return data_[i];
  }

  [[nodiscard]] T &operator[](size_t i) {
    checkBound(i);
    return data_[i];
  }
}; // class FixedVector

} // namespace mmm
