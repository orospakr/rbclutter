/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2007  Neil Roberts
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

#include <ruby.h>
#include <rbgobject.h>
#include <gst/gstobject.h>
#include <clutter-gst/clutter-gst-util.h>
#include <clutter-gst/clutter-gst-version.h>
#include <clutter/clutter-enum-types.h>

#include "rbcluttergst.h"

VALUE rbcltgst_c_clutter_gst = Qnil;

extern void rbcltgst_video_sink_init ();
extern void rbcltgst_video_texture_init ();
extern void rbcltgst_audio_init ();

void
rbcltgst_initialize_gst_object (VALUE obj, gpointer gstobj)
{
  /* Grab the floating reference if the object is a subclass of
     GstObject */
  if (GST_IS_OBJECT (gstobj))
    {
      gst_object_ref (gstobj);
      gst_object_sink (gstobj);
    }

  G_INITIALIZE (obj, gstobj);
}

void
rbcltgst_initialize_unowned (VALUE obj, gpointer gobj)
{
  /* If the object is a subclass of GInitiallyUnowned then grab the
     floating reference */
  if (G_IS_INITIALLY_UNOWNED (gobj))
    g_object_ref_sink (gobj);
  G_INITIALIZE (obj, gobj);
}

static void
rbcltgst_call_init ()
{
  VALUE init_args, prg_name;
  char **args_copy;
  int init_argc, i;
  ClutterInitError eval;

  /* Use the command line arguments from ARGV */
  init_args = rb_argv;

  Check_Type (init_args, T_ARRAY);
  init_argc = RARRAY (init_args)->len;

  /* Convert all of the arguments to strings */
  for (i = 0; i < init_argc; i++)
    rb_ary_store (init_args, i, StringValue (RARRAY (init_args)->ptr[i]));

  /* Make a copy of the array and prepend $0 */
  prg_name = rb_gv_get ("0");
  prg_name = StringValue (prg_name);
  args_copy = ALLOC_N (char *, init_argc + 1);
  args_copy[0] = RSTRING (prg_name)->ptr;
  for (i = 0; i < init_argc; i++)
    args_copy[i + 1] = RSTRING (RARRAY (init_args)->ptr[i])->ptr;

  /* Initialize clutter-gst */
  init_argc++;
  eval = clutter_gst_init (&init_argc, &args_copy);
  
  /* If it worked then copy the altered arguments back */
  if (eval == CLUTTER_INIT_SUCCESS)
    {
      rb_ary_clear (init_args);

      for (i = 1; i < init_argc; i++)
	rb_ary_push (init_args, rb_str_new2 (args_copy[i]));
    }

  free (args_copy);

  /* If it didn't work then throw an exception */
  if (eval != CLUTTER_INIT_SUCCESS)
    {
      VALUE init_error_class = rb_eval_string ("Clutter::InitError");
      VALUE ex = rb_exc_new2 (init_error_class, "Failed to initalise clutter-gst");
      rb_iv_set (ex, "@errnum", GENUM2RVAL (eval, CLUTTER_TYPE_INIT_ERROR));
      rb_exc_raise (ex);
    }
}

void
Init_clutter_gst ()
{
  VALUE clutter_klass;

  rb_require ("glib2");
  rb_require ("clutter");
  rb_require ("gst0.10");

  /* The base class init of some of the clutter-gst classes call some
     gst functions so unfortunatly defining the Ruby wrappers will
     crash if clutter_gst_init is not called first */
  rbcltgst_call_init ();

  clutter_klass = rb_define_module ("Clutter");
  rbcltgst_c_clutter_gst = rb_define_module_under (clutter_klass, "Gst");

  /* Version constants. Not defining a regular VERSION constant
     because under Ruby-GTK that constant refers to the version of the
     actual library binary, but there isn't a way to get that with
     Clutter yet. */
  rb_define_const (rbcltgst_c_clutter_gst, "BUILD_VERSION",
		   rb_ary_new3 (3, INT2FIX (CLUTTER_GST_MAJOR_VERSION),
				INT2FIX (CLUTTER_GST_MINOR_VERSION),
				INT2FIX (CLUTTER_GST_MICRO_VERSION)));

  rbcltgst_video_sink_init ();
  rbcltgst_video_texture_init ();
  rbcltgst_audio_init ();
}
