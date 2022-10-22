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

#include <concepts>

namespace mmm { // my magic matrix

template <typename T>
concept HasUnaryPlus = requires(T x) {
  { +x } -> std::convertible_to<T>;
};

template <typename T>
concept HasUnaryMinus = requires(T x) {
  { -x } -> std::convertible_to<T>;
};

template <typename T>
concept Addable = requires(T x, T y) {
  { x + y } -> std::convertible_to<T>;
};

template <typename T>
concept Substractable = requires(T x, T y) {
  { x - y } -> std::convertible_to<T>;
};

template <typename T>
concept Multipliable = requires(T x, T y) {
  { x *y } -> std::convertible_to<T>;
};

template <typename T>
concept Dividable = requires(T x, T y) {
  { x / y } -> std::convertible_to<T>;
};

template <typename T>
concept Arithmetic = HasUnaryPlus<T> && HasUnaryMinus<T> && Addable<T> &&
                     Substractable<T> && Multipliable<T> && Dividable<T> &&
                     std::assignable_from < std::remove_reference_t<T>
&, T > ;

} // namespace mmm
