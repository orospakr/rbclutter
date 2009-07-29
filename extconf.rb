#!/usr/bin/ruby

# Ruby bindings for the Clutter 'interactive canvas' library.
# Copyright (C) 2007  Neil Roberts
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301  USA
require 'pathname'

def show_fail
  print("*** Configuration failed due to a missing requirement\n")
  exit(1)
end

SUBDIRS = %w{ clutter clutter-gtk clutter-gst }

cwd = Pathname.getwd

successful_dirs = []
failed_dirs = []

# Run extconf.rb in each of the subdirectories
SUBDIRS.each do |dir|
  STDERR.print("Entering directory #{dir}\n")
  Dir.chdir(cwd.join(dir).to_s) do
    begin
      pid = Process.fork do
        load(cwd.join(dir, "extconf.rb").to_s, true)
      end
      Process.wait(pid)
      if $?.exitstatus == 0
        successful_dirs << dir
      else
        failed_dirs << dir
      end
    end
  end
  STDERR.print("Leaving directory #{dir}\n")
end

STDERR.print("\n" \
             "Succeeded: #{successful_dirs.size == 0 ? "none" : successful_dirs.join(' ')}\n" \
             "Failed:    #{failed_dirs.size == 0 ? "none" : failed_dirs.join(' ')}\n")

# Generate a parent makefile that forwards all of the rules to the
# makefiles in the subdirectories
RULES = %w{ all clean distclean install site-install }
File.open("Makefile", "w") do |mf|
  RULES.each do |rule|
    mf.print("#{rule}:\n")
    successful_dirs.each do |subdir|
      mf.print("\t$(MAKE) -C #{subdir} #{rule}\n")
    end
    mf.print("\n")
  end
  mf.print(".PHONY : #{RULES.join(" ")}\n")
end
