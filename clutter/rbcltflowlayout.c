/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2007-2008  Neil Roberts
 * Copyright (C) 2011       Andrew Clunis <andrew@orospakr.ca>
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
#include <clutter/clutter.h>

#include "rbclutter.h"

/* http://docs.clutter-project.org/docs/clutter/stable/ClutterFlowLayout.html */

static VALUE
rbclt_flow_layout_initialize (VALUE self, VALUE orientation)
{
  ClutterGravity orientation_val = RVAL2GENUM (orientation, CLUTTER_TYPE_FLOW_ORIENTATION);
  rbclt_initialize_unowned (self, clutter_flow_layout_new (orientation_val));

  return Qnil;
}

static VALUE
rbclt_flow_layout_set_homogeneous (VALUE self, VALUE homogeneous)
{
  ClutterFlowLayout *flow_layout = CLUTTER_FLOW_LAYOUT (RVAL2GOBJ (self));
  clutter_flow_layout_set_homogeneous (flow_layout, RVAL2CBOOL (homogeneous));

  return Qnil;
}

static VALUE
rbclt_flow_layout_get_homogeneous (VALUE self)
{
  ClutterFlowLayout *flow_layout = CLUTTER_FLOW_LAYOUT (RVAL2GOBJ (self));

  return CBOOL2RVAL (clutter_flow_layout_get_homogeneous (flow_layout));
}

static VALUE
rbclt_flow_layout_set_orientation (VALUE self, VALUE orientation)
{
  ClutterFlowLayout *flow_layout = CLUTTER_FLOW_LAYOUT (RVAL2GOBJ (self));
  ClutterGravity orientation_val = RVAL2GENUM (orientation, CLUTTER_TYPE_FLOW_ORIENTATION);

  clutter_flow_layout_set_orientation (flow_layout, orientation_val);

  return Qnil;
}

static VALUE
rbclt_flow_layout_get_orientation (VALUE self)
{
  ClutterFlowLayout *flow_layout = CLUTTER_FLOW_LAYOUT (RVAL2GOBJ (self));
  
  return GENUM2RVAL (clutter_flow_layout_get_orientation (flow_layout), CLUTTER_TYPE_FLOW_ORIENTATION);
}

static VALUE
rbclt_flow_layout_set_column_spacing (VALUE self, VALUE spacing)
{
  ClutterFlowLayout *flow_layout = CLUTTER_FLOW_LAYOUT (RVAL2GOBJ (self));

  clutter_flow_layout_set_column_spacing(flow_layout, NUM2DBL (spacing));
 
  return Qnil;
}

static VALUE
rbclt_flow_layout_get_column_spacing (VALUE self)
{
  ClutterFlowLayout *flow_layout = CLUTTER_FLOW_LAYOUT (RVAL2GOBJ (self));

  return DBL2NUM ( clutter_flow_layout_get_column_spacing (flow_layout));
}

static VALUE
rbclt_flow_layout_set_row_spacing (VALUE self, VALUE spacing)
{
  ClutterFlowLayout *flow_layout = CLUTTER_FLOW_LAYOUT (RVAL2GOBJ (self));

  clutter_flow_layout_set_row_spacing (flow_layout, NUM2DBL (spacing));
  
  return Qnil;
}

static VALUE
rbclt_flow_layout_get_row_spacing (VALUE self)
{
  ClutterFlowLayout *flow_layout = CLUTTER_FLOW_LAYOUT (RVAL2GOBJ (self));

  return DBL2NUM (clutter_flow_layout_get_row_spacing (flow_layout));
}

static VALUE
rbclt_flow_layout_set_column_width (VALUE self, VALUE min_width, VALUE max_width)
{
  ClutterFlowLayout *flow_layout = CLUTTER_FLOW_LAYOUT (RVAL2GOBJ (self));

  clutter_flow_layout_set_column_width (flow_layout, NUM2DBL (min_width), NUM2DBL (max_width));

  return Qnil;
}

static VALUE
rbclt_flow_layout_get_column_width (VALUE self)
{
  ClutterFlowLayout *flow_layout = CLUTTER_FLOW_LAYOUT (RVAL2GOBJ (self));
  gfloat min_width, max_width;
  VALUE answer = rb_hash_new();

  clutter_flow_layout_get_column_width (flow_layout, &min_width, &max_width);
  rb_hash_aset (answer, rb_intern("min"), rb_float_new (min_width));
  rb_hash_aset (answer, rb_intern("max"), rb_float_new (max_width));
  return answer;
}

static VALUE
rbclt_flow_layout_set_row_height (VALUE self, VALUE min_height, VALUE max_height)
{
  ClutterFlowLayout *flow_layout = CLUTTER_FLOW_LAYOUT (RVAL2GOBJ (self));

  clutter_flow_layout_set_row_height (flow_layout, NUM2DBL (min_height), NUM2DBL (max_height));

  return Qnil;
}

static VALUE
rbclt_flow_layout_get_row_height (VALUE self)
{
  ClutterFlowLayout *flow_layout = CLUTTER_FLOW_LAYOUT (RVAL2GOBJ (self));
  gfloat min_height, max_height;
  VALUE answer = rb_hash_new ();

  clutter_flow_layout_get_row_height (flow_layout, &min_height, &max_height);
  rb_hash_aset (answer, rb_intern("min"), rb_float_new (min_height));
  rb_hash_aset (answer, rb_intern("max"), rb_float_new (max_height));
  
  return answer;
}

void
rbclt_flow_layout_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_FLOW_LAYOUT, "FlowLayout", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_flow_layout_initialize, 1);
  rb_define_method (klass, "set_homogeneous", rbclt_flow_layout_set_homogeneous, 1);
  rb_define_method (klass, "get_homogeneous", rbclt_flow_layout_get_homogeneous, 0);
  rb_define_method (klass, "set_orientation", rbclt_flow_layout_set_orientation, 1);
  rb_define_method (klass, "get_orientation", rbclt_flow_layout_get_orientation, 0);
  rb_define_method (klass, "set_column_spacing", rbclt_flow_layout_set_column_spacing, 1);
  rb_define_method (klass, "get_column_spacing", rbclt_flow_layout_get_column_spacing, 0);
  rb_define_method (klass, "set_row_spacing", rbclt_flow_layout_set_row_spacing, 1);
  rb_define_method (klass, "get_row_spacing", rbclt_flow_layout_get_row_spacing, 0);
  rb_define_method (klass, "set_column_width", rbclt_flow_layout_set_column_width, 2);
  rb_define_method (klass, "get_column_width", rbclt_flow_layout_get_column_width, 0);
  rb_define_method (klass, "set_row_height", rbclt_flow_layout_set_row_height, 2);
  rb_define_method (klass, "get_row_height", rbclt_flow_layout_get_row_height, 0);
}
