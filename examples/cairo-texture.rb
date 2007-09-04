#!/usr/bin/ruby

# Adapted from the cairo-texture.py example in PyClutter

require 'clutter_cairo'

Clutter::init

stage = Clutter::Stage.get_default
stage.set_color(Clutter::Color.new(0xff, 0xcc, 0xcc, 0xff))
stage.set_size(400,300)

stage.signal_connect('button-press-event') { Clutter::main_quit }

cairo_tex = Clutter::Cairo.new(200,200)
cairo_tex.set_position((stage.width - 200) / 2,
                       (stage.height - 200) / 2)

# we obtain a cairo context from the Clutter::Cairo texture and then
# we can use it with the cairo primitives to draw on it.
cairo_tex.create do |context|
  # we scale the context to the size of the surface
  context.scale(200, 200)
  context.set_line_width(0.1)
  context.set_source_rgba(0, 0, 0, 1)
  context.translate(0.5, 0.5)
  context.arc(0, 0, 0.4, 0, Math::PI * 2)
  context.stroke

  # When this block is finished, the 'finish' method will
  # automatically be called on the context which will cause
  # clutter-cairo to update the texture
end

# Clutter::Cairo is a Clutter::Actor, so we can manipulate it as any
# other actor
center_x = cairo_tex.width / 2
center_y = cairo_tex.height / 2
cairo_tex.rotate_y(45.0, center_x, center_y)
stage << cairo_tex
cairo_tex.show
    
# Clutter::Cairo is also a Clutter::Texture, so we can save memory
# when dealing with multiple copies by simply cloning it and
# manipulating the clones
clone_tex = Clutter::CloneTexture.new(cairo_tex)
clone_tex.set_position((stage.width - 200) / 2,
                       (stage.height - 200) / 2)
clone_tex.rotate_y(-45.0, center_x, center_y)
stage << clone_tex
clone_tex.show

stage.show

Clutter::main
