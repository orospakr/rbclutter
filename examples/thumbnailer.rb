#!/usr/bin/ruby

# Ruby bindings for the Clutter 'interactive canvas' library.
# Copyright (C) 2007-2008  Neil Roberts
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
require 'gdk_pixbuf2'

class Thumbnailer
  THUMBNAIL_SIZE = 200

  def initialize(filenames)
    @filenames = filenames.dup

    # Next image to load
    @next_load = 0
    # Next actor to load to
    @next_load_actor = 0

    create_actors

    # Load a screenful of images
    load_screen

    # Start the first timeline
    @behaviours[0].alpha.timeline.rewind.start
  end

  private
  def create_actors
    stage = Clutter::Stage.get_default

    # Make enough actors and corresponding behaviours to fill the
    # screen
    @actors = []
    @behaviours = []

    start_x = stage.width / 2 - THUMBNAIL_SIZE / 2
    start_y = stage.height

    (stage.height / THUMBNAIL_SIZE).times do |y|
      (stage.width / THUMBNAIL_SIZE).times do |x|
        @actors << actor = Clutter::Texture.new
        actor.set_position(start_x, start_y)

        stage << actor
        
        timeline = Clutter::Timeline.new(30, 30)
        alpha = Clutter::Alpha.new(timeline, Clutter::Alpha::SINE_INC)
        @behaviours << behaviour = Clutter::BehaviourPath.new(alpha)
        timeline.signal_connect("completed") { on_completed(actor) }
        behaviour.append_knots(Clutter::Knot.new(start_x, start_y),
                               Clutter::Knot.new(x * THUMBNAIL_SIZE, y * THUMBNAIL_SIZE))
        behaviour.apply(actor)
      end
    end
  end

  private
  def on_completed(actor)
    if @removing
      Clutter::Stage.get_default.remove(actor)
      @removed_count += 1
      if @removed_count >= @actors.size
        @removing = nil
        @behaviours[0].alpha.timeline.rewind.start
      end
    else
      if index = @actors.index(actor)
        if index >= @next_load_actor - 1
          # Start removing the thumbnails
          @removing = @behaviours
          @removed_count = 0
          @behaviours.each do |b| 
            b.alpha.set_func(Clutter::Alpha::SINE_DEC)
            b.alpha.timeline.rewind.start
          end
          # Create a new set of actors
          create_actors
          # Load the next screen
          if @next_load >= @filenames.size
            Clutter::main_quit
          else
            @next_load_actor = 0
            load_screen
          end
        else
          # Start the next timeline
          @behaviours[index + 1].alpha.timeline.rewind.start
        end
      end
    end
  end

  private
  def load_screen
    while @next_load_actor < @actors.size and @next_load < @filenames.size
      # Pre-scale the image using GDK pixbuf so that it doesn't have
      # to be scaled every frame by clutter
      pixbuf = Gdk::Pixbuf.new(@filenames[@next_load],
                               THUMBNAIL_SIZE, THUMBNAIL_SIZE)
      @next_load += 1

      @actors[@next_load_actor].set_from_rgb_data(pixbuf.pixels,
                                                  pixbuf.has_alpha?,
                                                  pixbuf.width,
                                                  pixbuf.height,
                                                  pixbuf.rowstride,
                                                  pixbuf.has_alpha? ? 4 : 3, 0)
      
      @next_load_actor += 1
    end
  end
end

Clutter::init

if ARGV.size < 1
  bn = File.basename($0)
  STDERR.print("usage: #{File.basename($0)} <image>...\n")
  exit(1)
end

stage = Clutter::Stage.get_default
stage.color = Clutter::Color.new(0, 0, 0)
stage.fullscreen

stage.signal_connect("key_press_event") do |stage, event|
  if event.keyval == Clutter::Key_q || event.keyval == Clutter::Key_Q \
    || event.keyval == Clutter::Key_Escape
    Clutter::main_quit
  end
end

Thumbnailer.new(ARGV)

stage.show

Clutter::main
