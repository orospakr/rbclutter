#!/usr/bin/env ruby

require 'clutter'
Clutter::init()

# In order to see the weird assertion failure on destruction:
# gdb --args ruby ./layouts.rb --g-fatal-warnings

# I ran this against Clutter 1.6.

# This is the traceback I get:

# GLib-GObject-WARNING **: instance with invalid (NULL) class pointer
# aborting...

# Program received signal SIGABRT, Aborted.
# 0x00007ffff6c8fd05 in raise (sig=6) at ../nptl/sysdeps/unix/sysv/linux/raise.c:64
# 64	../nptl/sysdeps/unix/sysv/linux/raise.c: No such file or directory.
# 	in ../nptl/sysdeps/unix/sysv/linux/raise.c
# (gdb) bt
#0  0x00007ffff6c8fd05 in raise (sig=6) at ../nptl/sysdeps/unix/sysv/linux/raise.c:64
#1  0x00007ffff6c93ab6 in abort () at abort.c:92
#2  0x00007ffff6116a22 in g_logv () from /lib/x86_64-linux-gnu/libglib-2.0.so.0
#3  0x00007ffff6116aaf in g_log () from /lib/x86_64-linux-gnu/libglib-2.0.so.0
#4  0x00007ffff65f3052 in g_type_check_instance () from /usr/lib/x86_64-linux-gnu/libgobject-2.0.so.0
#5  0x00007ffff65e6536 in g_signal_handlers_destroy () from /usr/lib/x86_64-linux-gnu/libgobject-2.0.so.0
#6  0x00007ffff65cebb2 in g_object_unref () from /usr/lib/x86_64-linux-gnu/libgobject-2.0.so.0
#7  0x00007fffedbaff6b in set_layout_manager (gobject=0xc1e000) at ./clutter-box.c:423
#8  clutter_box_dispose (gobject=0xc1e000) at ./clutter-box.c:451
#9  0x00007ffff65d0ac0 in g_object_run_dispose () from /usr/lib/x86_64-linux-gnu/libgobject-2.0.so.0
#10 0x00007fffedb9a399 in clutter_actor_destroy (self=0xc1e000) at ./clutter-actor.c:4944
#11 0x00007fffedec34a4 in rbclt_actor_destroy (self=140737223488000) at rbcltactor.c:163
#12 0x00007ffff7b113fc in ?? () from /usr/lib/libruby1.8.so.1.8
#13 0x00007ffff7b115d7 in ?? () from /usr/lib/libruby1.8.so.1.8
#14 0x00007ffff7b16a93 in ?? () from /usr/lib/libruby1.8.so.1.8
#15 0x00007ffff7b112c3 in ?? () from /usr/lib/libruby1.8.so.1.8
#16 0x00007ffff7b115d7 in ?? () from /usr/lib/libruby1.8.so.1.8
#17 0x00007ffff7b16a93 in ?? () from /usr/lib/libruby1.8.so.1.8
#18 0x00007ffff7b0fbcd in ?? () from /usr/lib/libruby1.8.so.1.8
#19 0x00007ffff7b101af in ?? () from /usr/lib/libruby1.8.so.1.8
#20 0x00007ffff7b113fc in ?? () from /usr/lib/libruby1.8.so.1.8
#21 0x00007ffff7b115d7 in ?? () from /usr/lib/libruby1.8.so.1.8
#22 0x00007ffff7b11cc8 in ?? () from /usr/lib/libruby1.8.so.1.8
#23 0x00007ffff7b11f55 in rb_funcall () from /usr/lib/libruby1.8.so.1.8
#24 0x00007ffff6835e96 in ?? () from /usr/lib/ruby/1.8/x86_64-linux/glib2.so
#25 0x00007ffff610f4eb in ?? () from /lib/x86_64-linux-gnu/libglib-2.0.so.0
#26 0x00007ffff610dbcd in g_main_context_dispatch () from /lib/x86_64-linux-gnu/libglib-2.0.so.0
#27 0x00007ffff610e3a8 in ?? () from /lib/x86_64-linux-gnu/libglib-2.0.so.0
#28 0x00007ffff610e9f2 in g_main_loop_run () from /lib/x86_64-linux-gnu/libglib-2.0.so.0
#29 0x00007ffff230baf7 in gtk_main () from /usr/lib/libgtk-x11-2.0.so.0
#30 0x00007ffff28604b9 in ?? () from /usr/lib/ruby/1.8/x86_64-linux/gtk2.so
#31 0x00007ffff7b113fc in ?? () from /usr/lib/libruby1.8.so.1.8
#32 0x00007ffff7b115d7 in ?? () from /usr/lib/libruby1.8.so.1.8
#33 0x00007ffff7b16a93 in ?? () from /usr/lib/libruby1.8.so.1.8
#34 0x00007ffff7b1d109 in ?? () from /usr/lib/libruby1.8.so.1.8
#35 0x00007ffff7b1d155 in ruby_exec () from /usr/lib/libruby1.8.so.1.8
#36 0x00007ffff7b1fcf5 in ruby_run () from /usr/lib/libruby1.8.so.1.8
#37 0x0000000000400871 in main ()

class Window
  def destroy_clutter_box()
    @container.destroy()
  end

  def initialize()
    super()
    
    stage = Clutter::Stage.get_default
    stage.set_size(1024, 768)
    
    default = Clutter::Backend.default()
    default.set_resolution(96)

    @container = Clutter::Box.new(Clutter::BinLayout.new())


    stage.add(@container)
    boxfill = Clutter::BindConstraint.new(stage, Clutter::BindCoordinate::SIZE, 0)
    @container.add_constraint(boxfill)
    stage.show_all()
  end
end

w = Window.new()

puts "Wait 4 seconds..."
GLib::Timeout.add(4000) do
  puts "Attempting to destroy my ClutterBox!"
  w.destroy_clutter_box()
end

Clutter::main()
