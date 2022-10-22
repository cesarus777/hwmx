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

#include "Matrix.hpp"

#include <istream>

namespace mmm { // my magic matrix

template <typename T> [[nodiscard]] Matrix<T> magicScammer(std::istream &is) {
  auto is_valid = [](std::istream &is) -> bool {
    return (is.eof() || std::isspace(is.peek())) && !is.fail();
  };

  int n;
  is >> n;
  if (!is_valid(is))
    throw std::ios_base::failure("Invalid input format");

  if (n < 1)
    throw std::runtime_error(
        std::string("Invalid matrix size: ").append(std::to_string(n)));

  Matrix<T> m(n);
  std::ranges::for_each(m, [&is, is_valid](auto &elem) {
    is >> elem;
    if (!is_valid(is))
      throw std::ios_base::failure("Invalid input format");
  });

  return m;
}

} // namespace mmm
