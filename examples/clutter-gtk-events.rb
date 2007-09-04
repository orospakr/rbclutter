require 'clutter_gtk'
require 'gdk_pixbuf2'

# Adapted from the gtk-clutter-events.c example from clutter-gtk

class EventApp
  attr_accessor :window, :popup, :gtk_entry, :stage, :hand, :clutter_entry
end

# Set the clutter colors form the current gtk theme
def create_colors (app, stage, text)
  style = app.window.style

  # Set the stage color to base[NORMAL]
  color = style.bg(Gtk::STATE_NORMAL)
  stage.red   =  (color.red/65535.0) * 255;
  stage.green =  (color.green/65535.0) * 255;
  stage.blue  =  (color.blue/65535.0) * 255;
  
  # Now the text color
  color = style.text(Gtk::STATE_NORMAL)
  text.red   =  (color.red/65535.0) * 255;
  text.green =  (color.green/65535.0) * 255;
  text.blue  =  (color.blue/65535.0) * 255;
end

def on_x_changed(button, app)
  app.hand.rotate_x(button.value, app.hand.height, 0)
end

def on_y_changed(button, app)
  app.hand.rotate_y(button.value, app.hand.width / 2, 0)
end

def on_z_changed(button, app)
  app.hand.rotate_z(button.value, app.hand.width / 2, app.hand.height / 2)
end

def on_opacity_changed(button, app)
  app.hand.opacity = button.value
end

app = EventApp.new  
stage_color = Clutter::Color.new(255, 255, 255, 255)
text_color = Clutter::Color.new(0, 0, 0, 255)

Clutter::init
Gtk::init

# Create the inital gtk window and widgets, just like normal
widget = Gtk::Window.new(Gtk::Window::TOPLEVEL)
app.window = widget;
widget.title = "Gtk-Clutter Interaction demo"
widget.set_default_size(800, 600)
widget.resizable = true
widget.border_width = 12
widget.signal_connect("destroy") { Gtk::main_quit }
 
# Create our layout box
vbox = Gtk::VBox.new(false, 12)
app.window.add(vbox)

widget = Gtk::Entry.new
app.gtk_entry = widget
widget.text = "Enter some text"
vbox.pack_start(widget, false, false, 0)
widget.signal_connect("changed") do
  app.clutter_entry.text = app.gtk_entry.text
end

hbox = Gtk::HBox.new(false, 12)
vbox.pack_start(hbox, true, true, 0)

# Set up clutter & create our stage
create_colors(app, stage_color, text_color)
widget = Clutter::Gtk.new
hbox.pack_start(widget, true, true, 0)
app.stage = widget.stage
app.stage.set_color(stage_color)

# Create the main texture that the spin buttons manipulate
pixbuf = Gdk::Pixbuf.new("redhand.png");
actor = Clutter::Texture.new(pixbuf)
app.hand = actor
app.stage.add(actor)
actor.set_position(app.stage.width / 2 - actor.width / 2,
                   app.stage.height / 2 - actor.height / 2)

# Setup the clutter entry
actor = Clutter::Entry.new("Sans 10", "", text_color)
app.clutter_entry = actor
app.stage.add(actor)
actor.set_position(0, 0)
actor.set_size(500, 20)

# Create our adjustment widgets
vbox = Gtk::VBox.new(false, 6)
hbox.pack_start(vbox, false, false, 0)

box = Gtk::HBox.new(true, 6)
vbox.pack_start(box, false, true, 0)
label = Gtk::Label.new("Rotate x-axis")
box.pack_start(label, true, true, 0)
button = Gtk::SpinButton.new(0, 360, 1);
box.pack_start(button, true, true, 0)
button.signal_connect("value-changed") do |button|
  on_x_changed(button, app)
end

box = Gtk::HBox.new(true, 6)
vbox.pack_start(box, false, true, 0)
label = Gtk::Label.new("Rotate y-axis")
box.pack_start(label, true, true, 0)
button = Gtk::SpinButton.new(0, 360, 1);
box.pack_start(button, true, true, 0)
button.signal_connect("value-changed") do |button|
  on_y_changed(button, app)
end

box = Gtk::HBox.new(true, 6)
vbox.pack_start(box, false, true, 0)
label = Gtk::Label.new("Rotate z-axis")
box.pack_start(label, true, true, 0)
button = Gtk::SpinButton.new(0, 360, 1);
box.pack_start(button, true, true, 0)
button.signal_connect("value-changed") do |button|
  on_z_changed(button, app)
end

box = Gtk::HBox.new(true, 6)
vbox.pack_start(box, false, true, 0)
label = Gtk::Label.new("Adjust opacity")
box.pack_start(label, true, true, 0)
button = Gtk::SpinButton.new(0, 255, 1);
button.value = 255
box.pack_start(button, true, true, 0)
button.signal_connect("value-changed") do |button|
  on_opacity_changed(button, app)
end

app.stage.show_all
app.window.show_all

Gtk::main
