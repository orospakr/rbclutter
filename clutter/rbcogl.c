/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2008  Neil Roberts
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

#include <rbgobject.h>
#include <cogl/cogl.h>

#include "rbclutter.h"

extern void rb_cogl_consts_init (void);

VALUE rbclt_c_cogl;

static VALUE
rb_cogl_get_features (VALUE self)
{
  return INT2NUM (cogl_get_features ());
}

static VALUE
rb_cogl_features_available (VALUE self, VALUE features)
{
  return cogl_features_available (NUM2INT (features)) ? Qtrue : Qfalse;
}

static VALUE
rb_cogl_get_bitmasks (VALUE self)
{
  int red, green, blue, alpha;

  cogl_get_bitmasks (&red, &green, &blue, &alpha);

  return rb_ary_new3 (4, INT2NUM (red), INT2NUM (green),
		      INT2NUM (blue), INT2NUM (alpha));
}

static VALUE
rb_cogl_push_matrix (VALUE self)
{
  cogl_push_matrix ();

  return Qnil;
}

static VALUE
rb_cogl_pop_matrix (VALUE self)
{
  cogl_pop_matrix ();

  return Qnil;
}

static VALUE
rb_cogl_perspective (VALUE self, VALUE fovy, VALUE aspect,
		     VALUE z_near, VALUE z_far)
{
  cogl_perspective (rbclt_num_to_fixed (fovy),
		    rbclt_num_to_fixed (aspect),
		    rbclt_num_to_fixed (z_near),
		    rbclt_num_to_fixed (z_far));

  return Qnil;
}

static VALUE
rb_cogl_setup_viewport (VALUE self,
			VALUE width, VALUE height,
			VALUE fovy, VALUE aspect,
			VALUE z_near, VALUE z_far)
{
  cogl_setup_viewport (NUM2UINT (width), NUM2UINT (height),
		       rbclt_num_to_fixed (fovy),
		       rbclt_num_to_fixed (aspect),
		       rbclt_num_to_fixed (z_near),
		       rbclt_num_to_fixed (z_far));

  return Qnil;
}

static VALUE
rb_cogl_scale (VALUE self, VALUE x, VALUE y)
{
  cogl_scale (rbclt_num_to_fixed (x),
	      rbclt_num_to_fixed (y));

  return Qnil;
}

static VALUE
rb_cogl_translate (VALUE self, VALUE x, VALUE y, VALUE z)
{
  cogl_translatex (rbclt_num_to_fixed (x),
		   rbclt_num_to_fixed (y),
		   rbclt_num_to_fixed (z));

  return Qnil;
}

static VALUE
rb_cogl_rotate (VALUE self, VALUE angle, VALUE x, VALUE y, VALUE z)
{
  cogl_rotatex (rbclt_num_to_fixed (angle),
		NUM2INT (x), NUM2INT (y), NUM2INT (z));

  return Qnil;
}

static VALUE
rb_cogl_convert_matrix (const ClutterFixed mfixed[16])
{
  VALUE mfloat[16], *dst = mfloat + 16;
  const ClutterFixed *src;

  for (src = mfixed + 16; src > mfixed;)
    *(--dst) = rb_float_new (CLUTTER_FIXED_TO_FLOAT (*(--src)));

  return rb_ary_new4 (16, mfloat);
}

static VALUE
rb_cogl_get_projection_matrix (VALUE self)
{
  ClutterFixed mfixed[16];

  cogl_get_projection_matrix (mfixed);

  return rb_cogl_convert_matrix (mfixed);
}

static VALUE
rb_cogl_get_modelview_matrix (VALUE self)
{
  ClutterFixed mfixed[16];

  cogl_get_modelview_matrix (mfixed);

  return rb_cogl_convert_matrix (mfixed);
}

static VALUE
rb_cogl_get_viewport (VALUE self)
{
  ClutterFixed vfixed[4], *src;
  VALUE vfloat[4], *dst = vfloat + 4;

  cogl_get_viewport (vfixed);

  for (src = vfixed + 4; src > vfixed;)
    *(--dst) = rb_float_new (CLUTTER_FIXED_TO_FLOAT (*(--src)));

  return rb_ary_new4 (4, vfloat);
}

static VALUE
rb_cogl_clip_set (VALUE self, VALUE x_offset, VALUE y_offset,
		  VALUE width, VALUE height)
{
  cogl_clip_set (rbclt_num_to_fixed (x_offset),
		 rbclt_num_to_fixed (y_offset),
		 rbclt_num_to_fixed (width),
		 rbclt_num_to_fixed (height));

  return Qnil;
}

static VALUE
rb_cogl_clip_unset (VALUE self)
{
  cogl_clip_unset ();

  return Qnil;
}

static VALUE
rb_cogl_enable_depth_test (VALUE self, VALUE v)
{
  cogl_enable_depth_test (RTEST (v));

  return Qnil;
}

static VALUE
rb_cogl_alpha_func (VALUE self, VALUE func, VALUE ref)
{
  cogl_alpha_func (NUM2INT (func),
		   rbclt_num_to_fixed (ref));

  return Qnil;
}

static VALUE
rb_cogl_paint_init (VALUE self, VALUE color)
{
  cogl_paint_init ((ClutterColor *) RVAL2BOXED (color, CLUTTER_TYPE_COLOR));

  return Qnil;
}

void
rb_cogl_init ()
{
  rbclt_c_cogl = rb_define_module ("Cogl");

  rb_define_singleton_method (rbclt_c_cogl, "get_features",
			      rb_cogl_get_features, 0);
  rb_define_singleton_method (rbclt_c_cogl, "features_available?",
			      rb_cogl_features_available, 1);
  rb_define_singleton_method (rbclt_c_cogl, "get_bitmasks",
			      rb_cogl_get_bitmasks, 0);
  rb_define_singleton_method (rbclt_c_cogl, "push_matrix",
			      rb_cogl_push_matrix, 0);
  rb_define_singleton_method (rbclt_c_cogl, "pop_matrix",
			      rb_cogl_pop_matrix, 0);
  rb_define_singleton_method (rbclt_c_cogl, "perspective",
			      rb_cogl_perspective, 4);
  rb_define_singleton_method (rbclt_c_cogl, "setup_viewport",
			      rb_cogl_setup_viewport, 6);
  rb_define_singleton_method (rbclt_c_cogl, "scale",
			      rb_cogl_scale, 2);
  rb_define_singleton_method (rbclt_c_cogl, "translate",
			      rb_cogl_translate, 3);
  rb_define_singleton_method (rbclt_c_cogl, "rotate",
			      rb_cogl_rotate, 4);
  rb_define_singleton_method (rbclt_c_cogl, "get_projection_matrix",
			      rb_cogl_get_projection_matrix, 0);
  rb_define_singleton_method (rbclt_c_cogl, "get_modelview_matrix",
			      rb_cogl_get_modelview_matrix, 0);
  rb_define_singleton_method (rbclt_c_cogl, "get_viewport",
			      rb_cogl_get_viewport, 0);
  rb_define_singleton_method (rbclt_c_cogl, "clip_set",
			      rb_cogl_clip_set, 4);
  rb_define_singleton_method (rbclt_c_cogl, "clip_unset",
			      rb_cogl_clip_unset, 0);
  rb_define_singleton_method (rbclt_c_cogl, "enable_depth_test",
			      rb_cogl_enable_depth_test, 1);
  rb_define_singleton_method (rbclt_c_cogl, "alpha_func",
			      rb_cogl_alpha_func, 2);
  rb_define_singleton_method (rbclt_c_cogl, "paint_init",
			      rb_cogl_paint_init, 1);

  rb_cogl_consts_init ();
}
