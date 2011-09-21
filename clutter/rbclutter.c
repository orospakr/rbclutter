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

#include <ruby.h>
#include <rbgobject.h>
#include <glib-object.h>
#include <clutter/clutter.h>
/*#include <clutter/clutter-version.h>
#include <clutter/clutter-enum-types.h>
#include <clutter/clutter-fixed.h> */

#include "rbclutter.h"
#include "rbcltstagemanager.h"

VALUE rbclt_c_clutter = Qnil;
VALUE rbclt_c_clutter_error = Qnil;
static VALUE rbclt_c_clutter_init_error = Qnil;

extern void rbclt_main_init ();
extern void rbclt_actor_init ();
extern void rbclt_actor_box_init ();
extern void rbclt_geometry_init ();
extern void rbclt_stage_init ();
extern void rbclt_color_init ();
extern void rbclt_container_init ();
extern void rbclt_child_meta_init ();
extern void rbclt_text_init ();
extern void rbclt_vertex_init ();
extern void rbclt_group_init ();
extern void rbclt_texture_init ();
extern void rbclt_knot_init ();
extern void rbclt_timeline_init ();
extern void rbclt_alpha_init ();
extern void rbclt_animation_init ();
extern void rbclt_behaviour_init ();
extern void rbclt_behaviour_bspline_init ();
extern void rbclt_behaviour_depth_init ();
extern void rbclt_behaviour_ellipse_init ();
extern void rbclt_behaviour_opacity_init ();
extern void rbclt_behaviour_path_init ();
extern void rbclt_behaviour_rotate_init ();
extern void rbclt_behaviour_scale_init ();
extern void rbclt_perspective_init ();
extern void rbclt_event_init ();
extern void rbclt_keysyms_init ();
extern void rbclt_rectangle_init ();
extern void rbclt_feature_init ();
extern void rbclt_backend_init ();
extern void rbclt_media_init ();
extern void rbclt_shader_init ();
extern void rbclt_callback_func_init ();
extern void rbclt_frame_source_init ();
extern void rbclt_stage_manager_init ();
extern void rbclt_script_init ();
extern void rbclt_score_init ();
extern void rbclt_model_init ();
extern void rbclt_list_model_init ();
extern void rbclt_fog_init ();

extern void rb_cogl_init ();
extern void rb_cogl_texture_init ();
extern void rb_cogl_primitives_init ();
extern void rb_cogl_shader_init ();
extern void rb_cogl_program_init ();
extern void rb_cogl_offscreen_init ();

VALUE
rbclt_call_init_func (int argc, VALUE *argv, RBCLTInitFunc func)
{
  VALUE init_args, prg_name;
  char **args_copy;
  int init_argc, i;
  ClutterInitError eval;

  rb_scan_args (argc, argv, "01", &init_args);

  /* Default to using ARGV if the argument list wasn't given */
  if (init_args == Qnil)
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

  /* Initialize clutter */
  init_argc++;
  eval = (* func) (&init_argc, &args_copy);

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
      VALUE ex = rb_exc_new2 (rbclt_c_clutter_init_error,
                              "Failed to initalise Clutter");
      rb_iv_set (ex, "@errnum", GENUM2RVAL (eval, CLUTTER_TYPE_INIT_ERROR));
      rb_exc_raise (ex);
    }

  rbclt_stage_manager_init_global_mark ();

  return Qnil;
}

guint8
rbclt_num_to_guint8 (VALUE val)
{
  long num = rb_num2long (val);
  const char *s;

  if (num < 0)
    s = "small";
  else if (num > G_MAXUINT8)
    s = "big";
  else
    return (guint8) num;

  rb_raise (rb_eRangeError, "integer %ld too %s to convert to `guint8'",
            num, s);
}

guint16
rbclt_num_to_guint16 (VALUE val)
{
  long num = rb_num2long (val);
  const char *s;

  if (num < 0)
    s = "small";
  else if (num > G_MAXUINT16)
    s = "big";
  else
    return (guint16) num;

  rb_raise (rb_eRangeError, "integer %ld too %s to convert to `guint16'",
            num, s);
}

CoglFixed
rbclt_num_to_fixed (VALUE val)
{
  /* If the number is a fixed num then there's no need to go through a
     double first */
  if (FIXNUM_P (val))
    return COGL_INT_TO_FIXED (FIX2INT (val));
  else
    return COGL_FLOAT_TO_FIXED (NUM2DBL (val));
}

/* ClutterUnit */
/* rbclt_num_to_units (VALUE val) */
/* { */
/*   /\* If the number is a fixed num then there's no need to go through a */
/*      double first *\/ */
/*   if (FIXNUM_P (val)) */
/*     return CLUTTER_UNITS_FROM_INT (FIX2INT (val)); */
/*   else */
/*     return CLUTTER_UNITS_FROM_FLOAT (NUM2DBL (val)); */
/* } */

/* ClutterAngle */
/* rbclt_num_to_angle (VALUE val) */
/* { */
/*   return CLUTTER_ANGLE_FROM_DEG (NUM2DBL (val)); */
/* } */

void
rbclt_initialize_unowned (VALUE obj, gpointer gobj)
{
  /* If the object is a subclass of GInitiallyUnowned then grab the
     floating reference */
  if (G_IS_INITIALLY_UNOWNED (gobj))
    g_object_ref_sink (gobj);
  G_INITIALIZE (obj, gobj);
}

/* This is copied from the Clutter-Perl bindings. Now it's in two
   places so why isn't it in Glib? :) */
GType
rbclt_connect_flags_get_type (void)
{
  static GType etype = 0;

  if (etype == 0
      && (etype = g_type_from_name ("GConnectFlags")) == 0)
    {
      static const GFlagsValue values[] =
        {
          { G_CONNECT_AFTER, "G_CONNECT_AFTER", "after" },
          { G_CONNECT_SWAPPED, "G_CONNECT_SWAPPED", "swapped" },
          { 0, NULL, NULL }
        };

      etype = g_flags_register_static ("GConnectFlags", values);
    }

  return etype;
}

void
Init_clutter ()
{
  VALUE mglib;

  rb_require ("glib2");

  rbclt_c_clutter = rb_define_module ("Clutter");
  rbclt_c_clutter_error = rb_define_class_under (rbclt_c_clutter, "Error",
                                                 rb_eException);

  /* Version constants. Not defining a regular VERSION constant
     because under Ruby-GTK that constant refers to the version of the
     actual library binary, but there isn't a way to get that with
     Clutter yet. */
  rb_define_const (rbclt_c_clutter, "BINDING_VERSION",
                   rb_ary_new3 (3, INT2FIX (RBCLUTTER_MAJOR_VERSION),
                                INT2FIX (RBCLUTTER_MINOR_VERSION),
                                INT2FIX (RBCLUTTER_MICRO_VERSION)));
  rb_define_const (rbclt_c_clutter, "BUILD_VERSION",
                   rb_ary_new3 (3, INT2FIX (CLUTTER_MAJOR_VERSION),
                                INT2FIX (CLUTTER_MINOR_VERSION),
                                INT2FIX (CLUTTER_MICRO_VERSION)));

  G_DEF_CLASS (CLUTTER_TYPE_GRAVITY, "Gravity", rbclt_c_clutter);
  G_DEF_CONSTANTS (rbclt_c_clutter, CLUTTER_TYPE_GRAVITY, "CLUTTER_");

  G_DEF_CLASS (CLUTTER_TYPE_ROTATE_DIRECTION, "RotateDirection",
               rbclt_c_clutter);
  G_DEF_CONSTANTS (rbclt_c_clutter, CLUTTER_TYPE_ROTATE_DIRECTION, "CLUTTER_");

  G_DEF_CLASS (CLUTTER_TYPE_ROTATE_DIRECTION, "RotateAxis", rbclt_c_clutter);
  G_DEF_CONSTANTS (rbclt_c_clutter, CLUTTER_TYPE_ROTATE_AXIS, "CLUTTER_");

  G_DEF_CLASS (CLUTTER_TYPE_REQUEST_MODE, "RequestMode", rbclt_c_clutter);
  G_DEF_CONSTANTS (rbclt_c_clutter, CLUTTER_TYPE_REQUEST_MODE, "CLUTTER_");

  G_DEF_CLASS (CLUTTER_TYPE_PICK_MODE, "PickMode", rbclt_c_clutter);
  G_DEF_CONSTANTS (rbclt_c_clutter, CLUTTER_TYPE_PICK_MODE, "CLUTTER_");

  G_DEF_CLASS (CLUTTER_TYPE_FLOW_ORIENTATION, "FlowOrientation", rbclt_c_clutter);
  G_DEF_CONSTANTS (rbclt_c_clutter, CLUTTER_TYPE_FLOW_ORIENTATION, "CLUTTER_");

  mglib = rb_const_get (rb_cObject, rb_intern ("GLib"));
  /* This is only defined in later versions of the Ruby-glib
     bindings */
  if (!rb_const_defined (mglib, rb_intern ("ConnectFlags")))
    G_DEF_CLASS (rbclt_connect_flags_get_type (), "ConnectFlags", mglib);

  rbclt_c_clutter_init_error = rb_define_class_under (rbclt_c_clutter,
                                                      "InitError",
                                                      rbclt_c_clutter_error);
  rb_define_attr (rbclt_c_clutter_init_error, "errnum", Qtrue, Qfalse);
  G_DEF_CLASS (CLUTTER_TYPE_INIT_ERROR, "Code", rbclt_c_clutter_init_error);
  G_DEF_CONSTANTS (rbclt_c_clutter_init_error, CLUTTER_TYPE_INIT_ERROR,
                   "CLUTTER_INIT_");

  rbclt_main_init ();
  rbclt_actor_init ();
  rbclt_actor_box_init ();
  rbclt_geometry_init ();
  rbclt_vertex_init ();
  rbclt_container_init ();
  rbclt_child_meta_init ();
  rbclt_stage_init ();
  rbclt_color_init ();
  rbclt_text_init ();
  rbclt_group_init ();
  rbclt_texture_init ();
  rbclt_knot_init ();
  rbclt_timeline_init ();
  rbclt_alpha_init ();
  rbclt_animation_init ();
  rbclt_behaviour_init ();
  rbclt_behaviour_depth_init ();
  rbclt_behaviour_ellipse_init ();
  rbclt_behaviour_opacity_init ();
  rbclt_behaviour_path_init ();
  rbclt_behaviour_rotate_init ();
  rbclt_behaviour_scale_init ();
  rbclt_perspective_init ();
  rbclt_event_init ();
  rbclt_keysyms_init ();
  rbclt_rectangle_init ();
  rbclt_feature_init ();
  rbclt_backend_init ();
  rbclt_media_init ();
  rbclt_shader_init ();
  rbclt_callback_func_init ();
  rbclt_frame_source_init ();
  rbclt_stage_manager_init ();
  rbclt_script_init ();
  rbclt_score_init ();
  rbclt_model_init ();
  rbclt_list_model_init ();
  rbclt_fog_init ();
  rbclt_path_init ();
  rbclt_layout_manager_init ();
  rbclt_flow_layout_init ();

  /* rb_cogl_init (); */
  /* rb_cogl_texture_init (); */
  /* rb_cogl_primitives_init (); */
  /* rb_cogl_shader_init (); */
  /* rb_cogl_program_init (); */
  /* rb_cogl_offscreen_init (); */
}
