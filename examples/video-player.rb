#!/usr/bin/ruby

# Adapted from video-player.c in the clutter-gst examples (in a very
# un-Ruby way)

require 'clutter_gst'
require 'gdk_pixbuf2'

SEEK_H = 20
SEEK_W = 690

class VideoApp
  attr_accessor :vtexture, :control, :control_bg, :control_play, :control_pause
  attr_accessor :control_seek1, :control_seek2, :control_seekbar, :control_label
  attr_accessor :controls_showing, :paused, :controls_timeout
  attr_accessor :controls_tl, :effect1_tl
end

def control_tl_cb(timeline, frame_num, app)
  app.control.show_all
  (app.paused ? app.control_pause : app.control_play).hide
  (app.paused ? app.control_play : app.control_pause).show
  opacity = (frame_num * 0xde) / timeline.num_frames
  opacity = 0xde - opacity unless app.controls_showing
  app.control.opacity = opacity
end

def control_tl_complete_cb(timeline, app)
  app.control.hide_all unless app.controls_showing
  app.controls_timeout = nil
end

def controls_timeout_cb(app)
  show_controls(app, false)
  false
end

def show_controls(app, vis)
  return if app.controls_tl.playing?

  if vis and !app.controls_showing
    app.controls_showing = true
    app.controls_tl.start
    app.controls_timeout = GLib::Timeout.add(5 * 1000) { controls_timeout_cb(app) }
    return
  end

  if vis and app.controls_showing
    if app.controls_timeout
      GLib::Source.remove(app.controls_timeout)
      app.controls_timeout = GLib::Timeout.add(5 * 1000) { controls_timeout_cb(app) }
    end
    return
  end

  if !vis and app.controls_showing
    app.controls_showing = false
    app.controls_tl.start
    return
  end
end

def toggle_pause_state(app)
  if app.paused
    app.vtexture.playing = true
    app.paused = false
    app.control_play.hide
    app.control_pause.show
  else
    app.vtexture.playing = false
    app.paused = true
    app.control_pause.hide
    app.control_play.show
  end
end

def input_cb(stage, event, app)
  case event.event_type
  when Clutter::Event::MOTION
    show_controls(app, true)
  when Clutter::Event::BUTTON_PRESS
    if app.controls_showing
      actor = Clutter::Stage.get_default.get_actor_at_pos(event.x, event.y)
      print("got actor #{actor.actor_id} at #{event.x}x#{event.y}\n")

      case actor
      when app.control_pause, app.control_play
        toggle_pause_state(app)
      when app.control_seek1, app.control_seek2, app.control_seekbar
        (x, y) = app.control_seekbar.abs_position
        dist = event.x - x
        dist = 0 if dist < 0
        dist = SEEK_W if dist > SEEK_W
        pos = (dist * app.vtexture.duration) / SEEK_W
        app.vtexture.position = pos
      end
    end
  when Clutter::Event::KEY_RELEASE
    case event.symbol
    when Clutter::Key_q, Clutter::Key_Escape
      Clutter::main_quit
    when Clutter::Key_e
      app.effect1_tl.start unless app.effect1_tl.playing?
    else
      toggle_pause_state(app)
    end
  end
end

def size_change(texture, width, height)
  stage = Clutter::Stage.get_default
  new_height = (height * stage.width) / width
  if new_height < stage.height
    new_width = stage.width
    new_x = 0
    new_y = (stage.height - new_height) / 2
  else
    new_width = (width * stage.height) / height
    new_height = stage.height
    new_x = (stage.width - new_width) / 2
    new_y = 0
  end
  texture.set_position(new_x, new_y)
  texture.set_size(new_width, new_height)
end

def tick(vtex, app)
  position = vtex.position
  duration = vtex.duration

  unless duration == 0 or position == 0
    app.control_seekbar.set_size((position * SEEK_W) / duration, SEEK_H)
  end
end

def effect1_tl_cb(frame_num, app)
  app.vtexture.rotate_y(frame_num * 12, Clutter::Stage.get_default.width / 2, 0)
end

control_color1 = Clutter::Color.new(73, 74, 77, 0xee)
control_color2 = Clutter::Color.new(0xcc, 0xcc, 0xcc, 0xff)

if ARGV.size < 1
  STDERR.print("#{$0} <video file>\n")
  exit(1)
end

Gst::init

stage = Clutter::Stage.get_default
stage.fullscreen = true
stage.color = Clutter::Color.new(0, 0, 0, 0)

app = VideoApp.new
app.vtexture = Clutter::Gst::VideoTexture.new

# Dont let the underlying pixbuf dictate size
app.vtexture.sync_size = false
# Handle it ourselves so can scale up for fullscreen better
app.vtexture.signal_connect("size-change") do |texture, width, height|
  size_change(texture, width, height)
end

# Load up out video texture
app.vtexture.filename = ARGV[0]

# Create the control UI
app.control = Clutter::Group.new

app.control_bg = Clutter::Texture.new(Gdk::Pixbuf.new("vid-panel.png"))
app.control_play = Clutter::Texture.new(Gdk::Pixbuf.new("media-actions-start.png"))
app.control_pause = Clutter::Texture.new(Gdk::Pixbuf.new("media-actions-pause.png"))

app.control_seek1 = Clutter::Rectangle.new(control_color1)
app.control_seek2 = Clutter::Rectangle.new(control_color2)
app.control_seekbar = Clutter::Rectangle.new(control_color1)
app.control_seekbar.opacity = 0x99

app.control_label = Clutter::Label.new("Sans Bold 24", File.basename(ARGV[0]))
app.control_label.color = control_color1

app.control.add(app.control_bg, 
                app.control_play, 
                app.control_pause,
                app.control_seek1,
                app.control_seek2,
                app.control_seekbar,
                app.control_label)
app.control.opacity = 0xee

app.control_play.set_position(30, 30)
app.control_pause.set_position(30, 30)

app.control_seek1.set_size(SEEK_W+10, SEEK_H+10)
app.control_seek1.set_position(200, 100)
app.control_seek2.set_size(SEEK_W, SEEK_H)
app.control_seek2.set_position(205, 105)
app.control_seekbar.set_size(0, SEEK_H)
app.control_seekbar.set_position(205, 105)

app.control_label.set_position(200, 40)

# Add control UI to stage
stage.add(app.vtexture, app.control)

print("start\n")

x = (stage.width - app.control.width) / 2
y = stage.height - stage.height / 3

print("setting x = #{x}, y = #{y}, width = #{app.control.width}\n")

app.control.set_position(x, y)

print("stop\n");

# hook up a time line for fading controls
app.controls_tl = Clutter::Timeline.new(10, 30)
app.controls_tl.signal_connect("new-frame") { |tl, frame_num| control_tl_cb(tl, frame_num, app) }
app.controls_tl.signal_connect("completed") { |tl| control_tl_complete_cb(tl, app) }

app.effect1_tl = Clutter::Timeline.new(30, 90)
app.effect1_tl.signal_connect("new-frame") { |tl, frame_num| effect1_tl_cb(frame_num, app) }

# Hook up other events
stage.signal_connect("event") { |actor, event| input_cb(actor, event, app) }

app.vtexture.signal_connect("notify::position") { |vtex, pos| tick(vtex, app) }

app.vtexture.playing = true

stage.show_all

Clutter::main
