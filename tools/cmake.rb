#!/usr/bin/env ruby
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

$LOAD_PATH << File.dirname(__FILE__)

require 'util'

class Cmake
  def initialize(args)
    @build_type = args.key?(:build_type) ? args[:build_type] : 'Release'
    @source_root = args.key?(:source_root) ? args[:source_root] : raise('Source root expected')
    @build_root = args.key?(:build_root) ? args[:build_root] : "#{@source_root}/build"
    @install_root = args.key?(:install_root) ? args[:install_root] : "#{@source_root}/install"
    @build_sys = args.key?(:build_sys) ? args[:build_sys] : find_build_sys
    @verbose = args.key?(:verbose) ? args[:verbose] : false
  end

  def find_build_sys
    Util.which?('ninja') ? 'Ninja' : 'Unix Makefiles'
  end

  def cmake_task
    "cmake -G #{@build_sys} -S #{@source_root} -B #{@build_root} -DCMAKE_BUILD_TYPE=#{@build_type}"
  end

  def build_task
    "cmake --build #{@build_root}"
  end

  def ctest_task
    'ctest --output-on-failure'
  end

  def install_task
    "DESTDIR=#{@install_root} cmake --install #{@build_root} --prefix /"
  end

  def configure
    Util.run_wrapper cmake_task, @verbose
    self
  end

  def build
    Util.run_wrapper build_task, @verbose
    self
  end

  def ctest
    Dir.chdir @build_root do
      Util.run_wrapper ctest_task, @verbose
    end
    self
  end

  def install
    Util.run_wrapper install_task, @verbose
    self
  end
end
