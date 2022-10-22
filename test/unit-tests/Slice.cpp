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

#include <Slice.hpp>

#include <gtest/gtest.h>

#include <random>

template <typename T> class SliceTest : public ::testing::Test {
protected:
  using TestType = T;
  using Slice = mmm::Slice<TestType>;

  constexpr static auto min_size = 1;
  constexpr static auto max_size = 128;

  void SetUp() override {
    size = rand() % (max_size - min_size) + min_size;
    v.reserve(size);
    std::ranges::for_each(std::views::iota(0u, size), [this](auto x) {
      v.push_back(rand() % std::numeric_limits<int>::max());
    });
  }

  std::mt19937 rand{std::random_device{}()};
  size_t size;
  std::vector<TestType> v;
};

using SliceIntTest = SliceTest<int>;
using SliceFloatTest = SliceTest<float>;

TEST_F(SliceIntTest, CtorInt) {
  Slice s(v.data(), size, 0, 1, size);
  EXPECT_TRUE(std::ranges::equal(v, s));
  for (auto start : std::views::iota(0u, size)) {
    Slice mono(v.data(), size, start, 0, size);
    for (auto elem : mono)
      EXPECT_EQ(elem, v[start]);
  }
}

TEST_F(SliceFloatTest, CtorFloat) {
  Slice s(v.data(), size, 0, 1, size);
  EXPECT_TRUE(std::ranges::equal(v, s));
  for (auto start : std::views::iota(0u, size)) {
    Slice mono(v.data(), size, start, 0, size);
    for (auto elem : mono)
      EXPECT_FLOAT_EQ(elem, v[start]);
  }
}

TEST_F(SliceIntTest, CtorFailInt) {
  EXPECT_THROW(Slice(v.data(), size, 0, 1, size + 1), std::out_of_range);
  EXPECT_THROW(Slice(v.data(), size, 1, 1, size), std::out_of_range);
  if (size != 1)
    EXPECT_THROW(Slice(v.data(), size, 0, 2, size), std::out_of_range);
  else
    EXPECT_NO_THROW(Slice(v.data(), size, 0, 2, size));
  EXPECT_THROW(Slice(v.data(), size, 0, size, 2), std::out_of_range);
  EXPECT_THROW(Slice(v.data(), size, size, 0, 1), std::out_of_range);
}

TEST_F(SliceFloatTest, CtorFailFloat) {
  EXPECT_THROW(Slice(v.data(), size, 0, 1, size + 1), std::out_of_range);
  EXPECT_THROW(Slice(v.data(), size, 1, 1, size), std::out_of_range);
  if (size != 1)
    EXPECT_THROW(Slice(v.data(), size, 0, 2, size), std::out_of_range);
  else
    EXPECT_NO_THROW(Slice(v.data(), size, 0, 2, size));
  EXPECT_THROW(Slice(v.data(), size, 0, size, 2), std::out_of_range);
  EXPECT_THROW(Slice(v.data(), size, 0, 0, 0), std::out_of_range);
  EXPECT_THROW(Slice(v.data(), size, size, 0, 1), std::out_of_range);
}

TEST_F(SliceIntTest, SizeInt) {
  for (auto sliceSize : std::views::iota(1u, size)) {
    Slice s(v.data(), size, 0, 0, sliceSize);
    EXPECT_EQ(s.size(), sliceSize);
  }
}

TEST_F(SliceFloatTest, SizeFloat) {
  for (auto sliceSize : std::views::iota(1u, size)) {
    Slice s(v.data(), size, 0, 0, sliceSize);
    EXPECT_EQ(s.size(), sliceSize);
  }
}

TEST_F(SliceIntTest, IterInt) {
  constexpr size_t start = 0;
  constexpr size_t monoStride = 0;
  Slice mono(v.data(), size, start, monoStride, size);
  for (auto elem : mono)
    EXPECT_EQ(elem, v[start]);

  for (const auto stride : std::views::iota(1u, size)) {
    size_t sliceSize = size / stride + (size % stride != 0);
    Slice s(v.data(), size, start, stride, sliceSize);
    int counter = start;
    auto filter = [counter, stride](auto &elem) mutable {
      return (counter++ % stride) == 0;
    };
    EXPECT_TRUE(std::ranges::equal(v | std::views::filter(filter), s));
  }
}

TEST_F(SliceFloatTest, IterFloat) {
  constexpr size_t start = 0;
  constexpr size_t monoStride = 0;
  Slice mono(v.data(), size, start, monoStride, size);
  for (auto elem : mono)
    EXPECT_FLOAT_EQ(elem, v[start]);

  for (const auto stride : std::views::iota(1u, size)) {
    size_t sliceSize = size / stride + (size % stride != 0);
    Slice s(v.data(), size, start, stride, sliceSize);
    int counter = start;
    auto filter = [counter, stride](auto &elem) mutable {
      return (counter++ % stride) == 0;
    };
    EXPECT_TRUE(std::ranges::equal(v | std::views::filter(filter), s));
  }
}

TEST_F(SliceIntTest, ElemAccessInt) {
  constexpr size_t start = 0;
  constexpr size_t monoStride = 0;
  Slice mono(v.data(), size, start, monoStride, size);
  for (auto i : std::views::iota(0u, size))
    EXPECT_EQ(mono[i], v[start]);

  for (const auto stride : std::views::iota(1u, size)) {
    size_t sliceSize = size / stride + (size % stride != 0);
    Slice s(v.data(), size, start, stride, sliceSize);
    for (auto i : std::views::iota(0u, sliceSize))
      EXPECT_EQ(s[i], v[start + i * stride]);
  }
}

TEST_F(SliceFloatTest, ElemAccessFloat) {
  constexpr size_t start = 0;
  constexpr size_t monoStride = 0;
  Slice mono(v.data(), size, start, monoStride, size);
  for (auto i : std::views::iota(0u, size))
    EXPECT_FLOAT_EQ(mono[i], v[start]);

  for (const auto stride : std::views::iota(1u, size)) {
    size_t sliceSize = size / stride + (size % stride != 0);
    Slice s(v.data(), size, start, stride, sliceSize);
    for (auto i : std::views::iota(0u, sliceSize))
      EXPECT_FLOAT_EQ(s[i], v[start + i * stride]);
  }
}

TEST_F(SliceIntTest, AddAssSliceInt) {
  constexpr size_t start = 0;

  for (const auto stride : std::views::iota(1u, size)) {
    std::vector<TestType> vtmp = v;
    size_t sliceSize = size / stride + (size % stride != 0);
    Slice s(vtmp.data(), size, start, stride, sliceSize);
    s += s;
    for (auto i : std::views::iota(0u, sliceSize))
      EXPECT_EQ(s[i], v[start + i * stride] * 2);
  }
}

TEST_F(SliceFloatTest, AddAssSliceFloat) {
  constexpr size_t start = 0;

  for (const auto stride : std::views::iota(1u, size)) {
    std::vector<TestType> vtmp = v;
    size_t sliceSize = size / stride + (size % stride != 0);
    Slice s(vtmp.data(), size, start, stride, sliceSize);
    s += s;
    for (auto i : std::views::iota(0u, sliceSize))
      EXPECT_FLOAT_EQ(s[i], v[start + i * stride] * 2);
  }
}

TEST_F(SliceIntTest, AddAssFixedVectorInt) {
  constexpr size_t start = 0;

  for (const auto stride : std::views::iota(1u, size)) {
    std::vector<TestType> vtmp = v;
    size_t sliceSize = size / stride + (size % stride != 0);
    Slice s(vtmp.data(), size, start, stride, sliceSize);
    mmm::FixedVector<TestType> fv(v.begin(), sliceSize);
    s += fv;
    for (auto i : std::views::iota(0u, sliceSize))
      EXPECT_EQ(s[i], v[start + i * stride] + v[i]);
  }
}

TEST_F(SliceFloatTest, AddAssFixedVectorFloat) {
  constexpr size_t start = 0;

  for (const auto stride : std::views::iota(1u, size)) {
    std::vector<TestType> vtmp = v;
    size_t sliceSize = size / stride + (size % stride != 0);
    Slice s(vtmp.data(), size, start, stride, sliceSize);
    mmm::FixedVector<TestType> fv(v.begin(), sliceSize);
    s += fv;
    for (auto i : std::views::iota(0u, sliceSize))
      EXPECT_FLOAT_EQ(s[i], v[start + i * stride] + v[i]);
  }
}

TEST_F(SliceIntTest, IntMulSlice) {
  constexpr size_t start = 0;

  for (const auto stride : std::views::iota(1u, size)) {
    std::vector<TestType> vtmp = v;
    size_t sliceSize = size / stride + (size % stride != 0);
    Slice s(vtmp.data(), size, start, stride, sliceSize);
    auto mul = vtmp[0] * s;
    for (auto i : std::views::iota(0u, sliceSize))
      EXPECT_EQ(mul[i], v[start + i * stride] * v[0]);
  }
}

TEST_F(SliceFloatTest, FloatMulSlice) {
  constexpr size_t start = 0;

  for (const auto stride : std::views::iota(1u, size)) {
    std::vector<TestType> vtmp = v;
    size_t sliceSize = size / stride + (size % stride != 0);
    Slice s(vtmp.data(), size, start, stride, sliceSize);
    auto mul = vtmp[0] * s;
    for (auto i : std::views::iota(0u, sliceSize))
      EXPECT_FLOAT_EQ(mul[i], v[start + i * stride] * v[0]);
  }
}
