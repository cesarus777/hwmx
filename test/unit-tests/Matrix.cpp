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

#include <Matrix.hpp>

#include <gtest/gtest.h>

#include <random>

enum { MAX_DIM = 16 };

template <typename T> class MatrixTest : public ::testing::Test {
protected:
  using TestType = T;

  void SetUp() override {
    dim = rand() % MAX_DIM;
    size_t size = dim * dim;
    v.reserve(size);
    std::ranges::for_each(std::views::iota(0u, size), [this](auto x) {
      v.push_back(rand() % std::numeric_limits<int>::max());
    });
  }

  std::mt19937 rand{std::random_device{}()};
  size_t dim;
  std::vector<TestType> v;
};

using MatrixIntTest = MatrixTest<int>;
using MatrixFloatTest = MatrixTest<float>;

TEST_F(MatrixIntTest, CtorIntFromSize1) {
  mmm::Matrix<TestType> mi1(1);
  EXPECT_EQ(mi1.dim(), 1);
}

TEST_F(MatrixFloatTest, CtorFloatFromSize1) {
  mmm::Matrix<TestType> mf1(1);
  EXPECT_EQ(mf1.dim(), 1);
}

TEST_F(MatrixIntTest, CtorIntFromRandSize) {
  mmm::Matrix<TestType> mi(dim);
  EXPECT_EQ(mi.dim(), dim);
}

TEST_F(MatrixFloatTest, CtorFloatFromRandSize) {
  mmm::Matrix<TestType> mf(dim);
  EXPECT_EQ(mf.dim(), dim);
}

TEST_F(MatrixIntTest, CtorIntFromInputIter) {
  mmm::Matrix<TestType> mv(v.begin(), dim);
  EXPECT_TRUE(std::ranges::equal(v, mv));
}

TEST_F(MatrixFloatTest, CtorFloatFromInputIter) {
  mmm::Matrix<TestType> mv(v.begin(), dim);
  EXPECT_TRUE(std::ranges::equal(v, mv));
}

TEST_F(MatrixIntTest, CopyCtorIntFromFixedVector) {
  mmm::FixedVector<TestType> fv(v);
  mmm::Matrix<TestType> mv(fv);
  EXPECT_TRUE(std::ranges::equal(v, mv));
}

TEST_F(MatrixFloatTest, CopyCtorFloatFromFixedVector) {
  mmm::FixedVector<TestType> fv(v);
  mmm::Matrix<TestType> mv(fv);
  EXPECT_TRUE(std::ranges::equal(v, mv));
}

TEST_F(MatrixIntTest, MoveCtorIntFromFixedVector) {
  mmm::FixedVector<TestType> fv(v);
  mmm::Matrix<TestType> mv(std::move(fv));
  EXPECT_TRUE(std::ranges::equal(v, mv));
}

TEST_F(MatrixFloatTest, MoveCtorFloatFromFixedVector) {
  mmm::FixedVector<TestType> fv(v);
  mmm::Matrix<TestType> mv(std::move(fv));
  EXPECT_TRUE(std::ranges::equal(v, mv));
}

TEST_F(MatrixIntTest, CtorIntFromIterAndSentinel) {
  mmm::Matrix<TestType> m(v.begin(), v.end());
  EXPECT_TRUE(std::ranges::equal(v, m));
}

TEST_F(MatrixFloatTest, CtorFloatFromIterAndSentinel) {
  mmm::Matrix<TestType> m(v.begin(), v.end());
  EXPECT_TRUE(std::ranges::equal(v, m));
}

TEST_F(MatrixIntTest, CtorIntFromRange) {
  mmm::Matrix<TestType> m(v);
  EXPECT_TRUE(std::ranges::equal(v, m));
}

TEST_F(MatrixFloatTest, CtorFloatFromRange) {
  mmm::Matrix<TestType> m(v);
  EXPECT_TRUE(std::ranges::equal(v, m));
}

TEST_F(MatrixIntTest, CopyCtorInt) {
  mmm::Matrix<TestType> m(v);
  mmm::Matrix<TestType> other(m);
  EXPECT_TRUE(std::ranges::equal(other, m));
}

TEST_F(MatrixFloatTest, CopyCtorFloat) {
  mmm::Matrix<TestType> m(v);
  mmm::Matrix<TestType> other(m);
  EXPECT_TRUE(std::ranges::equal(other, m));
}

TEST_F(MatrixIntTest, CopyCtorIntFromFloat) {
  mmm::Matrix<TestType> m(v);
  mmm::Matrix<float> other(m);
  EXPECT_TRUE(std::ranges::equal(other, m));
}

TEST_F(MatrixFloatTest, CopyCtorFloatFromInt) {
  mmm::Matrix<TestType> m(v);
  mmm::Matrix<int> other(m);
  EXPECT_TRUE(std::ranges::equal(other, m));
}

TEST_F(MatrixIntTest, CopyAssInt) {
  mmm::Matrix<TestType> m(v);
  mmm::Matrix<TestType> other = m;
  EXPECT_TRUE(std::ranges::equal(other, m));
}

TEST_F(MatrixFloatTest, CopyAssFloat) {
  mmm::Matrix<TestType> m(v);
  mmm::Matrix<TestType> other = m;
  EXPECT_TRUE(std::ranges::equal(other, m));
}

TEST_F(MatrixIntTest, MoveCtorInt) {
  mmm::Matrix<TestType> m(v);
  mmm::Matrix<TestType> other(std::move(m));
  EXPECT_EQ(m.dim(), 0);
  EXPECT_TRUE(std::ranges::equal(other, v));
}

TEST_F(MatrixFloatTest, MoveCtorFloat) {
  mmm::Matrix<TestType> m(v);
  mmm::Matrix<TestType> other(std::move(m));
  EXPECT_EQ(m.dim(), 0);
  EXPECT_TRUE(std::ranges::equal(other, v));
}

TEST_F(MatrixIntTest, MoveAssInt) {
  mmm::Matrix<TestType> m(v);
  mmm::Matrix<TestType> other = std::move(m);
  EXPECT_EQ(m.dim(), 0);
  EXPECT_TRUE(std::ranges::equal(other, v));
}

TEST_F(MatrixFloatTest, MoveAssFloat) {
  mmm::Matrix<TestType> m(v);
  mmm::Matrix<TestType> other = std::move(m);
  EXPECT_EQ(m.dim(), 0);
  EXPECT_TRUE(std::ranges::equal(other, v));
}

TEST_F(MatrixIntTest, IntIter) {
  mmm::Matrix<TestType> m(v);
  EXPECT_TRUE(std::ranges::equal(m.begin(), m.end(), v.begin(), v.end()));
}

TEST_F(MatrixFloatTest, FloatIter) {
  mmm::Matrix<TestType> m(v);
  EXPECT_TRUE(std::ranges::equal(m.begin(), m.end(), v.begin(), v.end()));
}

TEST_F(MatrixIntTest, ElemAccessInt) {
  mmm::Matrix<TestType> m(v.data(), dim);
  for (auto i : std::views::iota(0u, dim)) {
    for (auto j : std::views::iota(0u, dim)) {
      EXPECT_EQ(m(i, j), v[i * dim + j]);
    }
  }

  const mmm::Matrix<TestType> cm(v.data(), dim);
  for (auto i : std::views::iota(0u, dim)) {
    for (auto j : std::views::iota(0u, dim)) {
      EXPECT_EQ(cm(i, j), v[i * dim + j]);
    }
  }
}

TEST_F(MatrixFloatTest, ElemAccessFloat) {
  mmm::Matrix<TestType> m(v.data(), dim);
  for (auto i : std::views::iota(0u, dim))
    for (auto j : std::views::iota(0u, dim))
      EXPECT_FLOAT_EQ(m(i, j), v[i * dim + j]);

  const mmm::Matrix<TestType> cm(v.data(), dim);
  for (auto i : std::views::iota(0u, dim))
    for (auto j : std::views::iota(0u, dim))
      EXPECT_FLOAT_EQ(cm(i, j), v[i * dim + j]);
}

TEST_F(MatrixIntTest, RowInt) {
  mmm::Matrix<TestType> m(v.data(), dim);
  for (auto i : std::views::iota(0u, dim)) {
    auto r = m.row(i);
    for (auto j : std::views::iota(0u, dim))
      EXPECT_EQ(m(i, j), r[j]);
  }

  const mmm::Matrix<TestType> cm(v.data(), dim);
  for (auto i : std::views::iota(0u, dim)) {
    auto r = m.row(i);
    for (auto j : std::views::iota(0u, dim))
      EXPECT_EQ(cm(i, j), r[j]);
  }
}

TEST_F(MatrixFloatTest, RowFloat) {
  mmm::Matrix<TestType> m(v.data(), dim);
  for (auto i : std::views::iota(0u, dim)) {
    auto r = m.row(i);
    for (auto j : std::views::iota(0u, dim))
      EXPECT_FLOAT_EQ(m(i, j), r[j]);
  }

  const mmm::Matrix<TestType> cm(v.data(), dim);
  for (auto i : std::views::iota(0u, dim)) {
    auto r = m.row(i);
    for (auto j : std::views::iota(0u, dim))
      EXPECT_FLOAT_EQ(cm(i, j), r[j]);
  }
}

TEST_F(MatrixIntTest, DetInt) {
  std::vector<TestType> v1 = {1};
  mmm::Matrix<TestType> m1(v1.data(), 1);
  EXPECT_EQ(m1.det(), 1);

  std::vector<TestType> v2({1, 0, 0, 1});
  mmm::Matrix<TestType> m2(v2.data(), 2);
  EXPECT_EQ(m2.det(), 1);

  std::vector<TestType> v3 = {1, 0, 0, 0, 1, 0, 0, 0, 1};
  mmm::Matrix<TestType> m3(v3.data(), 3);
  EXPECT_EQ(m3.det(), 1);
}

TEST_F(MatrixFloatTest, DetFloat) {
  std::vector<TestType> v1 = {1};
  mmm::Matrix<TestType> m1(v1.data(), 1);
  EXPECT_FLOAT_EQ(m1.det(), 1.0);

  std::vector<TestType> v2({1, 0, 0, 1});
  mmm::Matrix<TestType> m2(v2.data(), 2);
  EXPECT_FLOAT_EQ(m2.det(), 1.0);

  std::vector<TestType> v3 = {1, 0, 0, 0, 1, 0, 0, 0, 1};
  mmm::Matrix<TestType> m3(v3.data(), 3);
  EXPECT_FLOAT_EQ(m3.det(), 1.0);
}
