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

require 'build'
require 'util'

def repo_root
  Util.git_root_dir
end

$LOAD_PATH << "#{repo_root}/test"

require 'testsuite'

def run!(args)
  verbose = args.include?('-v') || args.include?('--verbose')
  Builder.new({ verbose: verbose }).configure.build.unit_test.install
  Testsuite::Context.new({ install_root: "#{repo_root}/install",
                          verbose: verbose }).run!
end

$PROGRAM_NAME == __FILE__ && run!(ARGV)
