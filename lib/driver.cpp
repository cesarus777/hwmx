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

#include <iostream>

#ifndef SCAN_TYPE
#define SCAN_TYPE int
#endif

int main() {
  try {
    auto m = mmm::magicScammer<SCAN_TYPE>(std::cin);
    std::cout << m.det() << std::endl;
  } catch (std::exception &e) {
    std::cerr << __FILE__ << ": Exception caught in main(): " << e.what()
              << std::endl;
  }
}
