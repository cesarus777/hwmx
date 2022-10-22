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

$LOAD_PATH << "#{File.dirname(__FILE__)}/gen"

require 'testgen'

module Testsuite
  class FailedTest < RuntimeError
    def initialize(msg = 'Test failed!')
      super
    end
  end

  class Context
    def initialize(opts)
      @install_root = opts.key?(:install_root) ? opts[:install_root] : './install'
      @verbose = opts.key?(:verbose) ? opts[:verbose] : false
    end

    def pretty_print_mtx(mtx)
      mtx.each do |row|
        row.each { |elem| print "#{elem} " }
        print "\n"
      end
    end

    def verify(mtx, det)
      expected_det = mtx.det()
      raise FailedTest until det != expected_det.to_s
    end

    def driver_for(type)
      prefix = type.to_s
      "#{prefix}_driver"
    end

    def run_case(driver_bin, mtx)
      driver_pipe = IO.popen(driver_bin, 'r+')
      driver_pipe.puts mtx.column_size
      mtx.each { |elem| driver_pipe.puts elem }
      driver_pipe.gets
    ensure
      driver_pipe && driver_pipe.close
    end

    def bin(name)
      "#{@install_root}/bin/#{name}"
    end

    def run_for(type)
      driver_name = driver_for(type)
      driver_bin = bin(driver_name)
      matrices = Testgen.emit_mtx({type: type})
      matrices.each do |mtx|
        det = run_case(driver_bin, mtx)
        verify(mtx, det)
        @verbose && puts("Success for size #{mtx.column_count} #{type.to_s}")
      rescue FailedTest
        puts "Test failed: for matrix with size #{mtx.column_count}"
        pretty_print_mtx(mtx)
        abort "Expected det == #{expected_det}, got det == #{got_det}"
      end
    end

    def run!
      [:int, :float, :double].each { |type| run_for type }
    end
  end
end

$PROGRAM_NAME == __FILE__ && Testsuite::Context.new({ intstall_root: "#{File.dirname(__FILE__)}/../install" }).run!
