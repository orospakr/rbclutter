#!/usr/bin/ruby

# Adapted from audio.c in the clutter-gst examples

require 'clutter_gst'

if ARGV.size < 1
  STDERR.print("Usage: #{File.basename($0)} URI\n")
  exit(1)
end

Gst::init

stage = Clutter::Stage.get_default
stage.color = Clutter::Color.new(0xcc, 0xcc, 0xcc)

# Make a label
label = Clutter::Label.new(nil, "Music")
label.set_position(100, 100)
stage << label

# Make a timeline
timeline = Clutter::Timeline.new(100, 30)
timeline.loop = true

# Set up audio player
audio = Clutter::Gst::Audio.new
audio.uri = ARGV[0]
audio.playing = true

# Start the timeline */
timeline.start

stage.show_all

Clutter::main
