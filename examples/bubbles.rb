#!/usr/bin/ruby

# Adapted from bubbles.py in the Python bindings

require 'clutter'
require 'clutter_cairo'
require 'cairo'

Clutter::init

N_BUBBLES = 50
BUBBLE_R = 128
SCREEN_W = 640
SCREEN_H = 480

def create_bubble
  bubble = Clutter::Cairo.new(BUBBLE_R*2, BUBBLE_R*2)

  bubble.create do |cr|
    cr.set_operator(Cairo::OPERATOR_CLEAR)
    cr.paint
    cr.set_operator(Cairo::OPERATOR_OVER)
    
    cr.arc(BUBBLE_R, BUBBLE_R, BUBBLE_R, 0.0, 2 * Math::PI)
    
    pattern = Cairo::RadialPattern.new(BUBBLE_R, BUBBLE_R, 0,
                                       BUBBLE_R, BUBBLE_R, BUBBLE_R)
    pattern.add_color_stop_rgba(0, 0.88, 0.95, 0.99, 0.1)
    pattern.add_color_stop_rgba(0.6, 0.88, 0.95, 0.99, 0.1)
    pattern.add_color_stop_rgba(0.8, 0.67, 0.83, 0.91, 0.2)
    pattern.add_color_stop_rgba(0.9, 0.5, 0.67, 0.88, 0.7)
    pattern.add_color_stop_rgba(1.0, 0.3, 0.43, 0.69, 0.8)
    
    cr.set_source(pattern)
    cr.fill_preserve
    
    pattern = Cairo::LinearPattern.new(0, 0, BUBBLE_R * 2, BUBBLE_R * 2)
    pattern.add_color_stop_rgba(0.0, 1.0, 1.0, 1.0, 0.0)
    pattern.add_color_stop_rgba(0.15, 1.0, 1.0, 1.0, 0.95)
    pattern.add_color_stop_rgba(0.3, 1.0, 1.0, 1.0, 0.0)
    pattern.add_color_stop_rgba(0.7, 1.0, 1.0, 1.0, 0.95)
    pattern.add_color_stop_rgba(1.0, 1.0, 1.0, 1.0, 0.0)
    
    cr.set_source(pattern)
    cr.fill
  end

  bubble
end    

class BubbleClone < Clutter::CloneTexture
  def initialize(texture, x, y)
    super(texture)
    set_position(x, y)
    @linear = 0
    @angular = 0
  end
    
  def notify_y(actor, pspec, stage)
    return if y > -height
    
    size = rand(BUBBLE_R * 2 - BUBBLE_R / 4) + BUBBLE_R / 4

    set_size(size, size)
    set_rotation(Clutter::Z_AXIS, rand * 360.0, size / 2, size / 2, 0)
    set_position(rand(SCREEN_W - BUBBLE_R),
                 rand(SCREEN_H * 3 - SCREEN_H * 2) + SCREEN_H * 2)
    opacity = rand(256 - 80) + 80

    @linear = rand * 2.5 + 0.5
    @angular = rand - 0.5
    @x = x.to_f
    @y = y.to_f
  end
    
  def new_frame(timeline, frame_num)
    angular = @angular * timeline.delta
    
    @y -= @linear * timeline.delta
    @x += angular
    set_position(@x, @y)

    angular += self.get_rotation(Clutter::Z_AXIS)[0]
    angular -= 360.0 while angular > 360
    angular += 360.0 while angular < 0
        
    radius = width / 2
    set_rotation(Clutter::Z_AXIS, angular, radius, radius, 0)
  end
end

srand

stage = Clutter::Stage.get_default
stage.set_size(SCREEN_W, SCREEN_H)
stage.set_color(Clutter::Color.new(0xe0, 0xf2, 0xfc, 0xff))
stage.signal_connect('destroy') { |*args| Clutter::main_quit }
    
timeline = Clutter::Timeline.new(1000)
timeline.loop = true
    
bubble = create_bubble

(0...N_BUBBLES).each do |bubble_i|
  clone = BubbleClone.new(bubble, bubble_i * BUBBLE_R * 2, -BUBBLE_R * 2)
        
  stage << clone
        
  clone.signal_connect('notify::y', stage, &clone.method("notify_y"))
  timeline.signal_connect('new-frame', &clone.method("new_frame"))
        
  clone.notify_y(nil, nil, stage)
end

stage.show_all
    
timeline.start
    
Clutter::main
