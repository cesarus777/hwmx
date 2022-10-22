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

#include <Scanner.hpp>

#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>

TEST(Scanner, ExpectedIntSuccsess1) {
  std::istringstream iss{"1 1"};
  auto m = mmm::magicScammer<int>(iss);
  EXPECT_EQ(m.dim(), 1);
  EXPECT_EQ(m(0, 0), 1);
}

TEST(Scanner, ExpectedIntSuccsess2) {
  std::istringstream iss{"2 1 2 3 4"};
  auto m = mmm::magicScammer<int>(iss);
  EXPECT_EQ(m.dim(), 2);
  for (int expect = 1; auto x : m)
    EXPECT_EQ(x, expect++);
}

TEST(Scanner, ExpectedFloatSuccsess1) {
  std::istringstream iss{"1 1.0"};
  auto m = mmm::magicScammer<float>(iss);
  EXPECT_EQ(m.dim(), 1);
  EXPECT_FLOAT_EQ(m(0, 0), 1.0f);
}

TEST(Scanner, ExpectedFloatSuccsess2) {
  std::istringstream iss{"2 1. 2 3.0 4"};
  auto m = mmm::magicScammer<float>(iss);
  EXPECT_EQ(m.dim(), 2);
  for (float expect = 1.0; auto x : m)
    EXPECT_FLOAT_EQ(x, expect++);
}

TEST(Scanner, ExpectedSizeFailure1) {
  std::istringstream iss{"0 0"};
  EXPECT_THROW(auto m = mmm::magicScammer<int>(iss), std::runtime_error);
}

TEST(Scanner, ExpectedSizeFailure2) {
  std::istringstream iss{"-1 1"};
  EXPECT_THROW(auto m = mmm::magicScammer<int>(iss), std::runtime_error);
}

TEST(Scanner, ExpectedSizeFailure3) {
  std::istringstream iss{"1.0 1"};
  EXPECT_THROW(auto m = mmm::magicScammer<int>(iss), std::ios_base::failure);
}

TEST(Scanner, ExpectedSizeFailure4) {
  std::istringstream iss{"1.0l 1"};
  EXPECT_THROW(auto m = mmm::magicScammer<int>(iss), std::ios_base::failure);
}

TEST(Scanner, ExpectedIntInputFailure1) {
  std::istringstream iss{"1 1.0"};
  EXPECT_THROW(auto m = mmm::magicScammer<int>(iss), std::ios_base::failure);
}

TEST(Scanner, ExpectedIntInputFailure2) {
  std::istringstream iss{"1 1.23"};
  EXPECT_THROW(auto m = mmm::magicScammer<int>(iss), std::ios_base::failure);
}

TEST(Scanner, ExpectedIntInputFailure3) {
  std::istringstream iss{"1 1l"};
  EXPECT_THROW(auto m = mmm::magicScammer<int>(iss), std::ios_base::failure);
}

TEST(Scanner, ExpectedFloatInputFailure1) {
  std::istringstream iss{"1 1.f"};
  EXPECT_THROW(auto m = mmm::magicScammer<float>(iss), std::ios_base::failure);
}
