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

#include <Concepts.hpp>

#include <complex>

#define TEST_ARITHMETICAL(type)                                                \
  static_assert(mmm::Arithmetic<type>, #type " expected to be arithmetical");  \
  static_assert(mmm::Arithmetic<std::complex<type>>,                           \
                "std::complex<" #type "> expected to be arithmetical")

TEST_ARITHMETICAL(signed char);
TEST_ARITHMETICAL(signed short);
TEST_ARITHMETICAL(signed int);
TEST_ARITHMETICAL(signed long);
TEST_ARITHMETICAL(signed long long);

TEST_ARITHMETICAL(unsigned char);
TEST_ARITHMETICAL(unsigned short);
TEST_ARITHMETICAL(unsigned int);
TEST_ARITHMETICAL(unsigned long);
TEST_ARITHMETICAL(unsigned long long);

TEST_ARITHMETICAL(float);
TEST_ARITHMETICAL(double);

#undef TEST_ARITHMETICAL
