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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <ruby.h>
#include <rbgobject.h>
#include <glib-object.h>
#include <clutter/clutter-version.h>
#include <clutter/clutter-enum-types.h>

#include "rbclutter.h"

VALUE rbclt_c_clutter = Qnil;
VALUE rbclt_c_clutter_error = Qnil;

extern void rbclt_main_init ();
extern void rbclt_actor_init ();
extern void rbclt_actor_box_init ();
extern void rbclt_geometry_init ();
extern void rbclt_stage_init ();
extern void rbclt_color_init ();
extern void rbclt_container_init ();
extern void rbclt_label_init ();
extern void rbclt_vertex_init ();
extern void rbclt_group_init ();
extern void rbclt_texture_init ();
extern void rbclt_knot_init ();
extern void rbclt_timeline_init ();
extern void rbclt_alpha_init ();
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
extern void rbclt_effect_init ();
extern void rbclt_entry_init ();
extern void rbclt_rectangle_init ();
extern void rbclt_box_init ();
extern void rbclt_padding_init ();
extern void rbclt_margin_init ();
extern void rbclt_layout_init ();
extern void rbclt_clone_texture_init ();
extern void rbclt_feature_init ();
extern void rbclt_backend_init ();
extern void rbclt_media_init ();

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

  rb_raise (rb_eRangeError, "integer %ld too %s to convert to `guint8'", num, s);
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

  rb_raise (rb_eRangeError, "integer %ld too %s to convert to `guint16'", num, s);
}

void
rbclt_initialize_unowned (VALUE obj, gpointer gobj)
{
  /* If the object is a subclass of GInitiallyUnowned then grab the
     floating reference */
  if (G_IS_INITIALLY_UNOWNED (gobj))
    g_object_ref_sink (gobj);
  G_INITIALIZE (obj, gobj);
}

void
Init_clutter ()
{
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
  G_DEF_CLASS (CLUTTER_TYPE_ROTATE_DIRECTION, "RotateDirection", rbclt_c_clutter);
  G_DEF_CONSTANTS (rbclt_c_clutter, CLUTTER_TYPE_ROTATE_DIRECTION, "CLUTTER_");
  G_DEF_CLASS (CLUTTER_TYPE_ROTATE_DIRECTION, "RotateAxis", rbclt_c_clutter);
  G_DEF_CONSTANTS (rbclt_c_clutter, CLUTTER_TYPE_ROTATE_AXIS, "CLUTTER_");

  rbclt_main_init ();
  rbclt_actor_init ();
  rbclt_actor_box_init ();
  rbclt_geometry_init ();
  rbclt_vertex_init ();
  rbclt_container_init ();
  rbclt_stage_init ();
  rbclt_color_init ();
  rbclt_label_init ();
  rbclt_group_init ();
  rbclt_texture_init ();
  rbclt_knot_init ();
  rbclt_timeline_init ();
  rbclt_alpha_init ();
  rbclt_behaviour_init ();
  rbclt_behaviour_bspline_init ();
  rbclt_behaviour_depth_init ();
  rbclt_behaviour_ellipse_init ();
  rbclt_behaviour_opacity_init ();
  rbclt_behaviour_path_init ();
  rbclt_behaviour_rotate_init ();
  rbclt_behaviour_scale_init ();
  rbclt_perspective_init ();
  rbclt_event_init ();
  rbclt_keysyms_init ();
  rbclt_effect_init ();
  rbclt_entry_init ();
  rbclt_rectangle_init ();
  rbclt_box_init ();
  rbclt_margin_init ();
  rbclt_padding_init ();
  rbclt_layout_init ();
  rbclt_clone_texture_init ();
  rbclt_feature_init ();
  rbclt_backend_init ();
  rbclt_media_init ();
}
