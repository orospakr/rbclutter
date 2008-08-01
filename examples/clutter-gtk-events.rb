require 'clutter_gtk'
require 'gdk_pixbuf2'

# Adapted from the gtk-clutter-events.c example from clutter-gtk

class EventApp
  attr_accessor :window, :popup, :gtk_entry, :stage, :hand, :clutter_entry
end

def on_x_changed(button, app)
  app.hand.set_rotation(Clutter::X_AXIS, button.value,
                        0, app.hand.height / 2, 0)
end

def on_y_changed(button, app)
  app.hand.set_rotation(Clutter::Y_AXIS, button.value,
                        app.hand.width / 2, 0, 0)
end

def on_z_changed(button, app)
  app.hand.set_rotation(Clutter::Z_AXIS, button.value,
                        app.hand.width / 2, app.hand.height / 2, 0)
end

def on_opacity_changed(button, app)
  app.hand.opacity = button.value
end

app = EventApp.new  

Clutter::Gtk.init

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
widget = Clutter::GtkEmbed.new
hbox.pack_start(widget, true, true, 0)
app.stage = widget.stage

# Create the main texture that the spin buttons manipulate
actor = Clutter::Texture.new("redhand.png")
app.hand = actor
app.stage.add(actor)
actor.set_position(app.stage.width / 2 - actor.width / 2,
                   app.stage.height / 2 - actor.height / 2)

# Setup the clutter entry
actor = Clutter::Entry.new("Sans 10", "")
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

# Set the colors from the GTK theme
app.stage.color = Clutter::Gtk.get_bg_color(app.window, Gtk::STATE_NORMAL)
app.clutter_entry.color \
= Clutter::Gtk.get_text_color(app.window, Gtk::STATE_NORMAL)

app.stage.show_all
app.window.show_all

Gtk::main
