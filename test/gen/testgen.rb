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

require 'matrix'
require 'optparse'

module Testgen
  def self.emit_mtx(opts)
    n_max = opts[:n_max] || 8
    type = opts[:type] || :int
    matrices = []
    (0...n_max).each do |i|
      dim = 2 ** i
      matrices.append(Matrix.build(dim) do
                        case type
                        when :int
                          min_int = -10
                          max_int =  10
                          rand(min_int...max_int)
                        when :float, :double
                          rand
                        end
                      end)
    end
    matrices
  end

  def self.run!(args)
    options = { type: :int, n_max: 8 }
    OptionParser.new do |opts|
      opts.banner = "Usage: testgen.rb [optsions]"

      opts.on("-m [MAX]", "--max [MAX]", Integer, "Specify max matrix size from 2**0 to 2**max") do |opt_max|
        raise OptionParser::InvalidArgument, 'max matrix size expected to be not negative' until opt_max > -1
        options[:n_max] = opt_max
      end

      opts.on("-t [TYPE]", "--type [TYPE]", String, "Specify data type for generating matrices") do |opt_type|
        case opt_type
        when 'int'
          options[:type] = :int
        when 'float'
          options[:type] = :float
        when 'double'
          options[:type] = :double
        else
          raise OptionParser::InvalidArgument, 'type expected to be either int, float or double'
        end
      end
    end.parse! args

    srand(Time.now.to_i)
    m = Testgen.emit_mtx(options)
    puts m.column_count
    m.each do |row|
      row.each { |elem| print "#{elem} " }
      print "\n"
    end
    puts m.det()
  rescue OptionParser::InvalidArgument => e
    abort "Bad usage: #{e.message}"
  end
end

$PROGRAM_NAME == __FILE__ && Testgen.run!(ARGV)
