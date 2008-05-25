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
class EventStruct
  attr_reader :name, :fields, :default_type, :types

  def initialize(name, default_type, types, *fields)
    @name = name
    @default_type = default_type
    @fields = {}
    @types = types
    i = 0
    while i < fields.size - 1
      @fields[fields[i]] = fields[i + 1]
      i += 2
    end
  end

  def var_name
    name_parts = @name.split(/(?=[A-Z])/)
    name_parts.slice!(0) if name_parts[0] == "Clutter"
    name_parts.map { |part| part.downcase }.join("_")
  end
end

EVENT_STRUCTS = [ 
  EventStruct.new("ClutterKeyEvent", "CLUTTER_KEY_PRESS",
                  %w{ CLUTTER_KEY_PRESS CLUTTER_KEY_RELEASE },
                  "time", "guint32",
                  "modifier_state", "ClutterModifierType",
                  "keyval", "guint",
                  "hardware_keycode", "guint16"),
  EventStruct.new("ClutterButtonEvent", "CLUTTER_BUTTON_PRESS",
                  %w{ CLUTTER_BUTTON_PRESS CLUTTER_BUTTON_RELEASE },
                  "time", "guint32",
                  "x", "gint",
                  "y", "gint",
                  "modifier_state", "ClutterModifierType",
                  "button", "guint32"),
  EventStruct.new("ClutterMotionEvent", "CLUTTER_MOTION",
                  %w{ CLUTTER_MOTION },
                  "time", "guint32",
                  "x", "gint",
                  "y", "gint",
                  "modifier_state", "ClutterModifierType"),
  EventStruct.new("ClutterScrollEvent", "CLUTTER_SCROLL",
                  %w{ CLUTTER_SCROLL },
                  "time", "guint32",
                  "x", "gint",
                  "y", "gint",
                  "direction", "ClutterScrollDirection",
                  "modifier_state", "ClutterModifierType"),
  EventStruct.new("ClutterStageStateEvent", "CLUTTER_STAGE_STATE",
                  %w{ CLUTTER_STAGE_STATE },
                  "changed_mask", "ClutterStageState",
                  "new_state", "ClutterStageState")
]

ENUMS = %w{ ClutterModifierType ClutterScrollDirection ClutterStageState }

def get_type_name(field_type)
  parts = field_type.split(/(?=[A-Z])/)
  parts.insert(1, "type")
  parts.map { |part| part.upcase }.join("_")
end

def cval2rbval(cval, field_type)
  case field_type
  when "guint32", "guint", "guint16"
    "UINT2NUM (#{cval})"
  when "gint"
    "INT2NUM (#{cval})"
  when /\AClutter/
    "GENUM2RVAL (#{cval}, #{get_type_name(field_type)})"
  else
    STDERR.print("Unknown type #{field_type}\n")
    exit(1)
  end
end

def rbval2cval(rbval, field_type)
  case field_type
  when "guint32", "guint"
    "NUM2UINT (#{rbval})"
  when "gint"
    "NUM2INT (#{rbval})"
  when "guint16"
    "rbclt_num_to_guint16 (#{rbval})"
  when /\AClutter/
    "RVAL2GENUM (#{rbval}, #{get_type_name(field_type)})"
  else
    STDERR.print("Unknown type #{field_type}\n")
    exit(1)
  end
end

def make_event_functions(out_file)
  EVENT_STRUCTS.each do |event|
    event.fields.each_pair do |field_name, field_type|
      cval = "((#{event.name} *) RVAL2BOXED (self, CLUTTER_TYPE_EVENT))->#{field_name}"
      out_file.print("static VALUE rbclt_#{event.var_name}_class;\n" \
                     "\n" \
                     "static VALUE\n" \
                     "rbclt_#{event.var_name}_get_#{field_name} (VALUE self)\n" \
                     "{\n" \
                     "  return #{cval2rbval(cval, field_type)};\n" \
                     "}\n" \
                     "\n" \
                     "static VALUE\n" \
                     "rbclt_#{event.var_name}_set_#{field_name} (VALUE self, VALUE value)\n" \
                     "{\n" \
                     "  #{cval} = #{rbval2cval('value', field_type)};\n" \
                     "  return self;\n" \
                     "}\n" \
                     "\n")
    end
    
    out_file.print("static VALUE\n" \
                   "rbclt_#{event.var_name}_initialize (int argc, VALUE *argv, VALUE self)\n" \
                   "{\n" \
                   "  VALUE event_type;\n" \
                   "  ClutterEvent event;\n" \
                   "  rb_scan_args (argc, argv, \"01\", &event_type);\n" \
                   "  memset (&event, '\\0', sizeof (event));\n" \
                   "  event.type = event_type == Qnil ? #{event.default_type}\n" \
                   "    : RVAL2GENUM (event_type, CLUTTER_TYPE_EVENT_TYPE);\n" \
                   "  G_INITIALIZE (self, &event);\n" \
                   "  return Qnil;\n" \
                   "}\n" \
                   "\n")
  end

  out_file.print("static VALUE\n" \
                 "rbclt_event_get_class (ClutterEventType type)\n" \
                 "{\n")
  first = true
  EVENT_STRUCTS.each do |event|
    event.types.each do |type|
      out_file.print("  ")
      if first
        first = false
      else
        out_file.print("else ")
      end
      out_file.print("if (type == #{type})\n" \
                     "    return rbclt_#{event.var_name}_class;\n")
    end
  end
  out_file.print("  else\n" \
                 "    return rbclt_event_class;\n" \
                 "}\n\n")
end

def make_enums(out_file)
  ENUMS.each do |enum|
    type_id = "CLUTTER_TYPE_" + enum.sub(/\AClutter/, "").split(/(?=[A-Z])/).join("_").upcase
    out_file.print("  G_DEF_CLASS (#{type_id}, \"#{enum.sub(/\AClutter/, '')}\", rbclt_event_class);\n" \
                   "  G_DEF_CONSTANTS (rbclt_event_class, #{type_id}, \"CLUTTER_\");\n")
  end
  out_file.print("\n")

end

def make_class_defs(out_file)
  EVENT_STRUCTS.each do |event|
    class_name = event.name.sub(/\AClutter/, "")
    out_file.print("  klass = rb_define_class_under (rbclt_c_clutter, " \
                   "\"#{class_name}\", rbclt_event_class);\n")
    event.fields.each_pair do |field_name, field_type|
      out_file.print("  rb_define_method (klass, \"#{field_name}\", " \
                     "rbclt_#{event.var_name}_get_#{field_name}, 0);\n" \
                     "  rb_define_method (klass, \"set_#{field_name}\", " \
                     "rbclt_#{event.var_name}_set_#{field_name}, 1);\n")
    end
    out_file.print("  rb_define_method (klass, \"initialize\", " \
                   "rbclt_#{event.var_name}_initialize, -1);\n" \
                   "  G_DEF_SETTERS (klass);\n" \
                   "  rbclt_#{event.var_name}_class = klass;\n" \
                   "\n")
  end
end

File.open("rbcltevent.c", "w") do |out_file|
  out_file.print("/* Automatically generated by #{File.basename($0)} */\n\n")

  File.open("rbcltevent.c.in", "r") do |in_file|
    in_file.each_line do |line|
      case line
      when %r{\A\s*/\*\s*EVENT_FUNCTIONS\s*\*/\s*\z}
        make_event_functions(out_file)
      when %r{\A\s*/\*\s*ENUMS\s*\*/\s*\z}
        make_enums(out_file)
      when %r{\A\s*/\*\s*CLASS_DEFS\s*\*/\s*\z}
        make_class_defs(out_file)
      else
        out_file.print(line)
      end
    end
  end
end
