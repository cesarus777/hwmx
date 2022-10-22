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

module Util
  def self.run_wrapper(cmd, verbose = false)
    _, log_w = IO.pipe
    if verbose
      success = system(cmd)
    else
      success = system(cmd, %i[out err] => log_w)
    end
    log_w.close
    raise "Command failed with exit #{$CHILD_STATUS.exitstatus}: #{cmd.split(' ', 2)[0]}" unless success

    return success
  end

  def self.git_root_dir
    log_r, log_w = IO.pipe
    success = system('git rev-parse --show-toplevel', %i[out err] => log_w)
    log_w.close
    dirname = log_r.read
    raise 'No git repo root was found' unless success

    return dirname.rstrip
  end

  def self.which?(cmd)
    `which #{cmd}`
    $?.success?
  end
end
