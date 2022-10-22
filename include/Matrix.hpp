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
#include "Slice.hpp"

#include <cmath>
#include <numeric>
#include <ostream>

namespace mmm { // my magic matrix

template <Arithmetic T> class Matrix {
public:
  using Elem = T;
  using Data = FixedVector<Elem>;

private:
  Data data_;
  size_t dim_;

public:
  constexpr Matrix(size_t n) : data_(n * n), dim_(n) {}

  template <typename I>
  Matrix(I i, size_t n) requires(std::input_iterator<I> ||
                                 std::ranges::input_range<I>)
      : data_(i, n * n), dim_(n) {}

  template <std::input_iterator I, std::sentinel_for<I> S>
  Matrix(I i, S s) : data_(i, s), dim_(std::sqrt(s - i)) {
    if (dim_ * dim_ != data_.size())
      throw std::runtime_error("Expected iterator range with square size");
  }

  Matrix(const Data &data) : data_(data), dim_(std::sqrt(data.size())) {
    if (dim_ * dim_ != data.size())
      throw std::runtime_error("Expected FixedVector with square size");
  }

  Matrix(Data &&data) : data_(std::move(data)), dim_(std::sqrt(data.size())) {
    if (dim_ * dim_ != data.size())
      throw std::runtime_error("Expected FixedVector with square size");
  }

  Matrix(const Matrix &m) : data_(m.data_), dim_(m.dim_) {}

  Matrix &operator=(const Matrix &m) {
    data_ = m.data_;
    dim_ = m.dim_;
    return *this;
  }

  template <typename U>
  Matrix(const Matrix<U> &m) requires(std::convertible_to<U, Elem>)
      : data_(m), dim_(m.dim()) {}

  template <typename U>
  Matrix &operator=(const Matrix &m) requires(std::is_convertible_v<U, Elem>) {
    if (dim_ != m.dim_)
      throw std::runtime_error(
          "Trying to copy assign matrix with different dim!");
    data_ = std::move(Data(m));
    dim_ = m.dim();
    return *this;
  }

  Matrix(Matrix &&m) : data_(std::move(m.data_)), dim_(m.dim_) {}

  Matrix &operator=(Matrix &&m) {
    data_ = std::move(m.data_);
    dim_ = m.dim_;
    m.dim_ = 0;
    return *this;
  }

  auto *begin() { return data_.begin(); }
  const auto *begin() const { return data_.begin(); }

  auto *end() { return data_.end(); }
  const auto *end() const { return data_.end(); }

  constexpr size_t dim() const { return dim_; }

  constexpr T operator()(size_t i, size_t j) const {
    return data_[i * dim_ + j];
  }

  constexpr T &operator()(size_t i, size_t j) { return data_[i * dim_ + j]; }

  auto row(size_t i) const {
    return Slice(data_.begin(), dim_ * dim_, dim_ * i, 1, dim_);
  }

  Matrix &diagonalize() {
    for (auto i : std::views::iota(1u, dim_)) {
      for (auto j : std::views::iota(i, dim_)) {
        T koeff = (*this)(j, i - 1) / (*this)(i - 1, i - 1);
        row(j) -= koeff * row(i - 1);
      }
    }
    return *this;
  }

  auto trace() const {
    return Slice(data_.begin(), dim_ * dim_, 0, dim_ + 1, dim_);
  }

  T det() const {
    using CompTy = typename std::conditional_t<std::integral<T>, double, T>;

    switch (dim_) {
    case 1:
      return data_[0];
    case 2:
      return data_[0] * data_[3] - data_[1] * data_[2];
    default:
      Matrix<CompTy> mtx4det(*this);
      mtx4det.diagonalize();
      auto tr = mtx4det.trace();
      return std::reduce(tr.begin(), tr.end(), Elem{1}, std::multiplies{});
    }
  }

  void dump(std::ostream &os) {
    for (auto i : std::views::iota(0u, dim_)) {
      for (auto j : std::views::iota(0u, dim_))
        os << (*this)(i, j) << " ";
      os << std::endl;
    }
  }
};

} // namespace mmm
