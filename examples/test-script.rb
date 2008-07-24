#!/usr/bin/ruby

# Ruby bindings for the Clutter 'interactive canvas' library.
# Copyright (C) 2008  Neil Roberts
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

require 'clutter'

Clutter::init

# Custom class to draw a smiley face. This can be instantiated from
# the ClutterScript too if it is registered as a GLib type
class SmileyFace < Clutter::Actor
  type_register

  def on_paint
    # Scale so that the coordinates 0 -> 100 extend to cover the size of
    # the actor
    Cogl::push_matrix
    Cogl::scale(allocation_geometry.width / 100.0,
                allocation_geometry.height / 100.0)

    # Blue eyes
    Cogl::color(0, 0, 255)

    # Left eye
    Cogl::path_ellipse(25, 25, 8, 8)
    Cogl::path_fill

    # Right eye
    Cogl::path_ellipse(75, 25, 8, 8)
    Cogl::path_fill

    # Mouth
    Cogl::color(255, 0, 0)
    Cogl::path_move_to(25, 60)
    Cogl::path_curve_to(30, 75,
                        70, 75,
                        75, 60)
    Cogl::path_curve_to(70, 90,
                        30, 90,
                        25, 60)
    Cogl::path_fill

    Cogl::pop_matrix
  end
end

SCRIPT = <<EOS
[
  { "id"           : "spin-timeline",
    "type"         : "ClutterTimeline",
    "loop"         : true,
    "num-frames"   : 120,
    "fps"          : 60
  },
  { "id"           : "move-timeline",
    "type"         : "ClutterTimeline",
    "loop"         : true,
    "num-frames"   : 240,
    "fps"          : 60
  },
  { "id"           : "spin-behaviour",
    "type"         : "ClutterBehaviourRotate",
    "angle-start"  : 0.0,
    "angle-end"    : 360.0,
    "axis"         : "z-axis",
    "alpha"        : { "timeline" : "spin-timeline", "function" : "sine" }
    "center-x"     : 50,
    "center-y"     : 50
  },
  { "id"           : "move-behaviour",
    "type"         : "ClutterBehaviourPath",
    "knots"        : [ [ 20, 10 ], [ 500, 0 ], [ 500, 260 ], [ 0, 260 ] ],
    "alpha"        : { "timeline" : "move-timeline", "function" : "sine" }
  },
  { "id"           : "face",
    "type"         : "SmileyFace",
    "width"        : 100,
    "height"       : 100,
    "x"            : 20,
    "y"            : 10,
    "behaviours"   : [ "spin-behaviour", "move-behaviour" ]
  },
  { "id"           : "hand",
    "type"         : "ClutterTexture",
    "filename"     : "redhand.png",
    "behaviours"   : [ "spin-behaviour" ],
    "x"            : 200,
    "y"            : 100
  },
  { "id"           : "spin-label",
    "type"         : "ClutterLabel",
    "text"         : "Spin",
    "font-name"    : "Sans 30",
    "x"            : 0,
    "y"            : 400
    "signals"      : [ { "name"    : "button-press-event",
                         "handler" : "toggle_timeline" } ],
    "reactive"     : true
  },
  { "id"           : "toplevel",
    "type"         : "ClutterGroup",
    "children"     : [ "face", "hand", "spin-label", "move-label" ]
  }
]
EOS

EXTRA_SCRIPT = <<EOS
[
  { "id"           : "move-label",
    "type"         : "ClutterLabel",
    "text"         : "Move",
    "font-name"    : "Sans 30",
    "x"            : 300,
    "y"            : 400
    "signals"      : [ { "name"    : "button-press-event",
                         "handler" : "toggle_timeline" } ],
    "reactive"     : true
  }
]
EOS

script = Clutter::Script.new
script.add_search_path(File.dirname($0))
script.load_from_data(SCRIPT)

# If a Ruby actor is created using ClutterScript then its constructor
# won't be called so we have to bind the signals here :(
script["face"].signal_connect('paint') { |actor| actor.on_paint }

# There's two says to use Clutter::Script#connect_signals. The most
# common is to create an object with a method for each handler. The
# signals will automatically be connected to the methods with the
# names given in the script.
class Manager
  def toggle_timeline(button, event, tl)
    if tl.playing?
      tl.pause
    else
      tl.start
    end
  end
end
# Connect the signals to the methods in a new Manager object and pass
# each method the spin timeline as an extra parameter
script.connect_signals(Manager.new, script["spin-timeline"])

# Add another label with another signal
script.load_from_data(EXTRA_SCRIPT)

# The other way to connect the signals is to pass a block to the
# connect_signals method. The block will be called with details of
# each signal so that it can be connected in any application specific
# way
script.connect_signals do |obj, signal, handler, connect_object, flags|
  case handler
  when "toggle_timeline"
    obj.signal_connect(signal) do
      tl = script["move-timeline"]
      if tl.playing?
        tl.pause
      else
        tl.start
      end
    end
  end
end

stage = Clutter::Stage.get_default
stage << script["toplevel"]

stage.show

Clutter::main
