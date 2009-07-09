/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2007-2008  Neil Roberts
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301  USA
 */

#include <clutter/clutter.h>
#include <rbgobject.h>

#include "rbclutter.h"
#include "rbcltcallbackfunc.h"

typedef guint (* ThreadSourceFunc) (gint priority,
                                    guint interval,
                                    GSourceFunc func,
                                    gpointer data,
                                    GDestroyNotify notify);

static VALUE
rbclt_init (int argc, VALUE *argv)
{
  return rbclt_call_init_func (argc, argv, clutter_init);
}

static VALUE
rbclt_get_debug_enabled ()
{
  return clutter_get_debug_enabled () ? Qtrue : Qfalse;
}

static VALUE
rbclt_get_show_fps ()
{
  return clutter_get_show_fps () ? Qtrue : Qfalse;
}

static VALUE
rbclt_get_timestamp ()
{
  return ULONG2NUM (clutter_get_timestamp ());
}

static VALUE
rbclt_main ()
{
  clutter_main ();
  return Qnil;
}

static VALUE
rbclt_main_quit ()
{
  clutter_main_quit ();
  return Qnil;
}

static VALUE
rbclt_main_level ()
{
  return INT2NUM (clutter_main_level ());
}

static VALUE
rbclt_redraw (VALUE self, VALUE stage)
{
  clutter_redraw (CLUTTER_STAGE (RVAL2GOBJ (stage)));

  return Qnil;
}

static VALUE
rbclt_do_event (VALUE self, VALUE event)
{
  clutter_do_event ((ClutterEvent *) RVAL2BOXED (event, CLUTTER_TYPE_EVENT));

  return Qnil;
}

static VALUE
rbclt_set_motion_events_enabled (VALUE self, VALUE val)
{
  clutter_set_motion_events_enabled (RTEST (val));

  return Qnil;
}

static VALUE
rbclt_get_motion_events_enabled (VALUE self)
{
  return clutter_get_motion_events_enabled () ? Qtrue : Qfalse;
}

static VALUE
rbclt_set_motion_events_frequency (VALUE self, VALUE val)
{
  clutter_set_motion_events_frequency (NUM2UINT (val));

  return Qnil;
}

static VALUE
rbclt_get_motion_events_frequency (VALUE self)
{
  return UINT2NUM (clutter_get_motion_events_frequency ());
}

static VALUE
rbclt_set_default_frame_rate (VALUE self, VALUE val)
{
  clutter_set_default_frame_rate (NUM2UINT (val));

  return Qnil;
}

static VALUE
rbclt_get_default_frame_rate (VALUE self)
{
  return UINT2NUM (clutter_get_default_frame_rate ());
}

static VALUE
rbclt_grab_pointer (VALUE self, VALUE actor)
{
  clutter_grab_pointer (CLUTTER_ACTOR (RVAL2GOBJ (actor)));

  return Qnil;
}

static VALUE
rbclt_ungrab_pointer (VALUE self)
{
  clutter_ungrab_pointer ();

  return Qnil;
}

static VALUE
rbclt_get_pointer_grab (VALUE self)
{
  return GOBJ2RVAL (clutter_get_pointer_grab ());
}

static VALUE
rbclt_grab_keyboard (VALUE self, VALUE actor)
{
  clutter_grab_keyboard (CLUTTER_ACTOR (RVAL2GOBJ (actor)));

  return Qnil;
}

static VALUE
rbclt_ungrab_keyboard (VALUE self)
{
  clutter_ungrab_keyboard ();

  return Qnil;
}

static VALUE
rbclt_get_keyboard_grab (VALUE self)
{
  return GOBJ2RVAL (clutter_get_keyboard_grab ());
}

static VALUE
rbclt_clear_glyph_cache (VALUE self)
{
  clutter_clear_glyph_cache ();

  return Qnil;
}

static VALUE
rbclt_set_use_mipmapped_text (VALUE self, VALUE val)
{
  clutter_set_use_mipmapped_text (RTEST (val));

  return Qnil;
}

static VALUE
rbclt_get_use_mipmapped_text (VALUE self)
{
  return clutter_get_use_mipmapped_text () ? Qtrue : Qfalse;
}

static VALUE
rbclt_threads_init (VALUE self)
{
  clutter_threads_init ();

  return Qnil;
}

static VALUE
rbclt_threads_enter (VALUE self)
{
  clutter_threads_enter ();

  return Qnil;
}

static VALUE
rbclt_threads_leave (VALUE self)
{
  clutter_threads_leave ();

  return Qnil;
}

static VALUE
rbclt_threads_synchronize (VALUE self)
{
  int state = 0;
  VALUE ret;

  clutter_threads_enter ();

  ret = rb_protect (rb_yield, Qnil, &state);

  clutter_threads_leave ();

  if (state)
    rb_jump_tag (state);

  return ret;
}

static gboolean
rbclt_threads_callback (gpointer data)
{
  return RTEST (rbclt_callback_func_invoke ((RBCLTCallbackFunc *) data,
                                            0, NULL));
}

static VALUE
rbclt_threads_do_add_timeout (int argc, VALUE *argv,
                              ThreadSourceFunc source_func)
{
  VALUE priority_arg, interval_arg, func;
  gint priority;
  guint id, interval;
  RBCLTCallbackFunc *func_wrapper;

  rb_scan_args (argc, argv, "11&", &interval_arg, &priority_arg, &func);

  priority = NIL_P (priority_arg) ? G_PRIORITY_DEFAULT : NUM2INT (priority_arg);
  interval = NUM2UINT (interval_arg);
  func_wrapper = rbclt_callback_func_new (func);

  id = (* source_func)
    (priority, interval, rbclt_threads_callback, func_wrapper,
     (GDestroyNotify) rbclt_callback_func_destroy);

  return UINT2NUM (id);
}

static VALUE
rbclt_threads_add_timeout (int argc, VALUE *argv, VALUE self)
{
  return rbclt_threads_do_add_timeout (argc, argv,
                                       clutter_threads_add_timeout_full);
}

static VALUE
rbclt_threads_add_frame_source (int argc, VALUE *argv, VALUE self)
{
  return rbclt_threads_do_add_timeout (argc, argv,
                                       clutter_threads_add_frame_source_full);
}

void
rbclt_main_init ()
{
  VALUE klass;

  rb_define_module_function (rbclt_c_clutter, "init", rbclt_init, -1);

  rb_define_const (rbclt_c_clutter, "PRIORITY_REDRAW",
                   INT2NUM (CLUTTER_PRIORITY_REDRAW));

  rb_define_module_function (rbclt_c_clutter, "debug_enabled?",
                             rbclt_get_debug_enabled, 0);
  rb_define_module_function (rbclt_c_clutter, "show_fps?",
                             rbclt_get_show_fps, 0);
  rb_define_module_function (rbclt_c_clutter, "timestamp",
                             rbclt_get_timestamp, 0);
  rb_define_module_function (rbclt_c_clutter, "main", rbclt_main, 0);
  rb_define_module_function (rbclt_c_clutter, "main_quit", rbclt_main_quit, 0);
  rb_define_module_function (rbclt_c_clutter, "main_level",
                             rbclt_main_level, 0);
  rb_define_module_function (rbclt_c_clutter, "redraw", rbclt_redraw, 1);
  rb_define_module_function (rbclt_c_clutter, "do_event",
                             rbclt_do_event, 1);
  rb_define_module_function (rbclt_c_clutter, "set_motion_events_enabled",
                             rbclt_set_motion_events_enabled, 1);
  rb_define_module_function (rbclt_c_clutter, "motion_events_enabled?",
                             rbclt_get_motion_events_enabled, 0);
  rb_define_module_function (rbclt_c_clutter, "set_motion_events_frequency",
                             rbclt_set_motion_events_frequency, 1);
  rb_define_module_function (rbclt_c_clutter, "motion_events_frequency",
                             rbclt_get_motion_events_frequency, 0);
  rb_define_module_function (rbclt_c_clutter, "set_default_frame_rate",
                             rbclt_set_default_frame_rate, 1);
  rb_define_module_function (rbclt_c_clutter, "default_frame_rate",
                             rbclt_get_default_frame_rate, 0);
  rb_define_module_function (rbclt_c_clutter, "grab_pointer",
                             rbclt_grab_pointer, 1);
  rb_define_module_function (rbclt_c_clutter, "ungrab_pointer",
                             rbclt_ungrab_pointer, 0);
  rb_define_module_function (rbclt_c_clutter, "pointer_grab",
                             rbclt_get_pointer_grab, 0);
  rb_define_module_function (rbclt_c_clutter, "grab_keyboard",
                             rbclt_grab_keyboard, 1);
  rb_define_module_function (rbclt_c_clutter, "ungrab_keyboard",
                             rbclt_ungrab_keyboard, 0);
  rb_define_module_function (rbclt_c_clutter, "keyboard_grab",
                             rbclt_get_keyboard_grab, 0);
  rb_define_module_function (rbclt_c_clutter, "clear_glyph_cache",
                             rbclt_clear_glyph_cache, 0);
  rb_define_module_function (rbclt_c_clutter, "set_use_mipmapped_text",
                             rbclt_set_use_mipmapped_text, 1);
  rb_define_module_function (rbclt_c_clutter, "use_mipmapped_text?",
                             rbclt_get_use_mipmapped_text, 0);

  klass = rb_define_module_under (rbclt_c_clutter, "Threads");

  rb_define_module_function (klass, "init", rbclt_threads_init, 0);
  rb_define_module_function (klass, "enter", rbclt_threads_enter, 0);
  rb_define_module_function (klass, "leave", rbclt_threads_leave, 0);
  rb_define_module_function (klass, "synchronize",
                             rbclt_threads_synchronize, 0);
  rb_define_module_function (klass, "add_timeout",
                             rbclt_threads_add_timeout, -1);
  rb_define_module_function (klass, "add_frame_source",
                             rbclt_threads_add_frame_source, -1);
}
