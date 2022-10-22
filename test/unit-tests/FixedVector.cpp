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

#include <FixedVector.hpp>

#include <gtest/gtest.h>

#include <random>

enum { MAX_SIZE = 128 };

template <typename T> class FixedVectorTest : public ::testing::Test {
protected:
  using TestType = T;

  void SetUp() override {
    size = rand() % MAX_SIZE;
    v.reserve(size);
    std::ranges::for_each(std::views::iota(0u, size), [this](auto x) {
      v.push_back(rand() % std::numeric_limits<int>::max());
    });
  }

  std::mt19937 rand{std::random_device{}()};
  size_t size;
  std::vector<TestType> v;
};

using FixedVectorIntTest = FixedVectorTest<int>;
using FixedVectorFloatTest = FixedVectorTest<float>;

TEST_F(FixedVectorIntTest, CtorIntFromSize1) {
  mmm::FixedVector<TestType> vi1(1);
  EXPECT_EQ(vi1.size(), 1);
}

TEST_F(FixedVectorFloatTest, CtorFloatFromSize1) {
  mmm::FixedVector<TestType> vf1(1);
  EXPECT_EQ(vf1.size(), 1);
}

TEST_F(FixedVectorIntTest, CtorIntFromRandSize) {
  mmm::FixedVector<TestType> vi(size);
  EXPECT_EQ(vi.size(), size);
}

TEST_F(FixedVectorFloatTest, CtorFloatFromRandSize) {
  mmm::FixedVector<TestType> vf(size);
  EXPECT_EQ(vf.size(), size);
}

TEST_F(FixedVectorIntTest, CtorIntFromInputIter) {
  mmm::FixedVector<TestType> fv(v.begin(), size);
  EXPECT_TRUE(std::ranges::equal(v, fv));
}

TEST_F(FixedVectorFloatTest, CtorFloatFromInputIter) {
  mmm::FixedVector<TestType> fv(v.begin(), size);
  EXPECT_TRUE(std::ranges::equal(v, fv));
}

TEST_F(FixedVectorIntTest, CtorIntFromInputRange) {
  mmm::FixedVector<TestType> fv(v, size);
  EXPECT_TRUE(std::ranges::equal(v, fv));
}

TEST_F(FixedVectorFloatTest, CtorFloatFromInputRange) {
  mmm::FixedVector<TestType> fv(v, size);
  EXPECT_TRUE(std::ranges::equal(v, fv));
}

TEST_F(FixedVectorIntTest, CtorIntFromIterAndSentinel) {
  mmm::FixedVector<TestType> fv(v.begin(), v.end());
  EXPECT_TRUE(std::ranges::equal(v, fv));
}

TEST_F(FixedVectorFloatTest, CtorFloatFromIterAndSentinel) {
  mmm::FixedVector<TestType> fv(v.begin(), v.end());
  EXPECT_TRUE(std::ranges::equal(v, fv));
}

TEST_F(FixedVectorIntTest, CtorIntFromRange) {
  mmm::FixedVector<TestType> fv(v);
  EXPECT_TRUE(std::ranges::equal(v, fv));
}

TEST_F(FixedVectorFloatTest, CtorFloatFromRange) {
  mmm::FixedVector<TestType> fv(v);
  EXPECT_TRUE(std::ranges::equal(v, fv));
}

TEST_F(FixedVectorIntTest, CopyCtorInt) {
  mmm::FixedVector<TestType> fv(v);
  mmm::FixedVector<TestType> other(fv);
  EXPECT_TRUE(std::ranges::equal(other, fv));
}

TEST_F(FixedVectorFloatTest, CopyCtorFloat) {
  mmm::FixedVector<TestType> fv(v);
  mmm::FixedVector<TestType> other(fv);
  EXPECT_TRUE(std::ranges::equal(other, fv));
}

TEST_F(FixedVectorIntTest, CopyCtorIntFromFloat) {
  mmm::FixedVector<TestType> fv(v);
  mmm::FixedVector<float> other(fv);
  EXPECT_TRUE(std::ranges::equal(other, fv));
}

TEST_F(FixedVectorFloatTest, CopyCtorFloatFromInt) {
  mmm::FixedVector<TestType> fv(v);
  mmm::FixedVector<int> other(fv);
  EXPECT_TRUE(std::ranges::equal(other, fv));
}

TEST_F(FixedVectorIntTest, CopyAssInt) {
  mmm::FixedVector<TestType> fv(v);
  mmm::FixedVector<TestType> other = fv;
  EXPECT_TRUE(std::ranges::equal(other, fv));
}

TEST_F(FixedVectorFloatTest, CopyAssFloat) {
  mmm::FixedVector<TestType> fv(v);
  mmm::FixedVector<TestType> other = fv;
  EXPECT_TRUE(std::ranges::equal(other, fv));
}

TEST_F(FixedVectorIntTest, MoveCtorInt) {
  mmm::FixedVector<TestType> fv(v);
  mmm::FixedVector<TestType> other(std::move(fv));
  EXPECT_EQ(fv.size(), 0);
  EXPECT_TRUE(std::ranges::equal(other, v));
}

TEST_F(FixedVectorFloatTest, MoveCtorFloat) {
  mmm::FixedVector<TestType> fv(v);
  mmm::FixedVector<TestType> other(std::move(fv));
  EXPECT_EQ(fv.size(), 0);
  EXPECT_TRUE(std::ranges::equal(other, v));
}

TEST_F(FixedVectorIntTest, MoveAssInt) {
  mmm::FixedVector<TestType> fv(v);
  mmm::FixedVector<TestType> other = std::move(fv);
  EXPECT_EQ(fv.size(), 0);
  EXPECT_TRUE(std::ranges::equal(other, v));
}

TEST_F(FixedVectorFloatTest, MoveAssFloat) {
  mmm::FixedVector<TestType> fv(v);
  mmm::FixedVector<TestType> other = std::move(fv);
  EXPECT_EQ(fv.size(), 0);
  EXPECT_TRUE(std::ranges::equal(other, v));
}

TEST_F(FixedVectorIntTest, CheckBoundInt) {
  mmm::FixedVector<TestType> fv(v);
  for (auto i : std::views::iota(0u, size))
    EXPECT_NO_THROW(fv.checkBound(i));
  EXPECT_THROW(fv.checkBound(size), std::out_of_range);
}

TEST_F(FixedVectorFloatTest, CheckBoundFloat) {
  mmm::FixedVector<TestType> fv(v);
  for (auto i : std::views::iota(0u, size))
    EXPECT_NO_THROW(fv.checkBound(i));
  EXPECT_THROW(fv.checkBound(size), std::out_of_range);
}

TEST_F(FixedVectorIntTest, ElemAccessInt) {
  mmm::FixedVector<TestType> fv(v);
  for (auto i : std::views::iota(0u, size))
    EXPECT_EQ(fv[i], v[i]);
  EXPECT_THROW(auto x = fv[size], std::out_of_range);
}

TEST_F(FixedVectorFloatTest, ElemAccessFloat) {
  mmm::FixedVector<TestType> fv(v);
  for (auto i : std::views::iota(0u, size))
    EXPECT_EQ(fv[i], v[i]);
  EXPECT_THROW(auto x = fv[size], std::out_of_range);
}
