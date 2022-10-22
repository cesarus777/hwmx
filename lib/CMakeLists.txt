# ---------------------------------------------------------------------------- #
# Copyright 2022 Yuly Tarasov
#
# This file is part of hwmx.
#
# hwmx is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# hwmx is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# hwmx. If not, see <https://www.gnu.org/licenses/>.
# ---------------------------------------------------------------------------- #

set(DRIVER_TYPES_LIST int float double)

foreach(TYPE IN LISTS DRIVER_TYPES_LIST)
  set(DRIVER_NAME "${TYPE}_driver")
  add_executable(${DRIVER_NAME} driver.cpp)
  target_compile_definitions(${DRIVER_NAME} PUBLIC SCAN_TYPE=${TYPE})

  install(TARGETS ${DRIVER_NAME}
    RUNTIME COMPONENT ${DRIVER_NAME}
  )
endforeach()
