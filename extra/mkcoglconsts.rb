#!/usr/bin/ruby

if ARGV.size != 2
  STDERR.print("Usage: #{File.basename($0)}: <cogl_header_file> " \
               "<cogl_defines>\n")
  exit(1)
end

print <<EOS
/* Automatically generated by #{File.basename($0)} */

/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2007-2008  Neil Roberts
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301  USA
 */

#include <rbgobject.h>
#include <cogl/cogl.h>

#include "rbclutter.h"

void
rb_cogl_consts_init (void)
{
  VALUE enum_module;

EOS

#include "rbclutter.h"

state = :enum_start
values = []
enums = []

File.open(ARGV[0]) do |infile|
  infile.each_line do |line|
    line.chomp!

    case state
    when :enum_start
      state = :open_bracket if line.match(/\A\s*typedef\s+enum\s*\z/)
    when :open_bracket
      state = :value_start if line.match(/\A\s*\{\s*\z/)
    when :value_start, :value_end
      if md = line.match(/\A\s*\}\s*([A-Z0-9a-z]+)\s*;\s*\z/)
        enum_name = md[1]
        enums << [ enum_name, *values ]
        values.clear
        state = :enum_start
      elsif state == :value_start
        if md = line.match(/\A\s*([A-Z_][A-Z0-9_]*)(.*,)?/)
          values << md[1]
          state = :value_end if md[2].nil?
        end
      else
        state = :value_start if line.match(/,/)
      end
    else
      raise "internal error"
    end
  end
end

enums.each do |enum|
  name = enum[0]
  values = enum[1..-1]

  name_parts = name.split(/(?=[A-Z])/).map { |x| x.upcase }
  common_parts = name_parts.length

  class_name = name.sub(/\ACogl/, "")

  values.each do |value|
    value_parts = value.split(/_/)
    common_count = 0
    while common_count < value_parts.length \
      && common_count < name_parts.size \
      && name_parts[common_count] == value_parts[common_count]
      common_count += 1
    end

    if common_count < common_parts
      common_parts = common_count
    end
  end

  print("  enum_module = rb_define_module_under (rbclt_c_cogl, \"#{class_name}\");\n")

  values.each do |value|
    value_name = value.split(/_/)[common_parts..-1].join("_")
    print("  rb_define_const (enum_module, \"#{value_name}\",\n" \
          "                   INT2FIX (#{value}));\n")
  end
end

defines = []

File.open(ARGV[1]) do |infile|
  infile.each_line do |line|
    if md = line.match(/\A\s*\#\s*define\s+(CGL_[A-Z90-9_]+)/)
      defines << md[1]
    end
  end
end

defines.sort!

defines.each do |define|
  short_name = define[4..-1]
  print("#ifdef GL_#{short_name}\n" \
        "  rb_define_const (rbclt_c_cogl, \"#{short_name}\",\n" \
        "                   INT2FIX (#{define}));\n" \
        "#endif\n")
end

print("}\n")