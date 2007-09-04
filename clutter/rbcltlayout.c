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

#include <rbgobject.h>
#include <clutter/clutter-layout.h>
#include <clutter/clutter-enum-types.h>

#include "rbclutter.h"

static VALUE
rbclt_layout_get_layout_flags (VALUE self)
{
  ClutterLayout *layout = CLUTTER_LAYOUT (RVAL2GOBJ (self));
  return GFLAGS2RVAL (clutter_layout_get_layout_flags (layout),
		      CLUTTER_TYPE_LAYOUT_FLAGS);
}

static VALUE
rbclt_layout_width_for_height (VALUE self, VALUE height)
{
  ClutterLayout *layout = CLUTTER_LAYOUT (RVAL2GOBJ (self));
  gint width;
  clutter_layout_width_for_height (layout, &width, NUM2INT (height));
  return INT2NUM (width);
}

static VALUE
rbclt_layout_height_for_width (VALUE self, VALUE width)
{
  ClutterLayout *layout = CLUTTER_LAYOUT (RVAL2GOBJ (self));
  gint height;
  clutter_layout_height_for_width (layout, NUM2INT (width), &height);
  return INT2NUM (height);
}

static VALUE
rbclt_layout_natural_request (VALUE self)
{
  ClutterLayout *layout = CLUTTER_LAYOUT (RVAL2GOBJ (self));
  gint width, height;
  clutter_layout_natural_request (layout, &width, &height);
  return rb_ary_new3 (2, INT2NUM (width), INT2NUM (height));
}

static VALUE
rbclt_layout_tune_request (VALUE self, VALUE given_width, VALUE given_height)
{
  ClutterLayout *layout = CLUTTER_LAYOUT (RVAL2GOBJ (self));
  gint width, height;
  clutter_layout_tune_request (layout, NUM2INT (given_width), NUM2INT (given_height),
			       &width, &height);
  return rb_ary_new3 (2, INT2NUM (width), INT2NUM (height));
}

void
rbclt_layout_init ()
{
  VALUE klass = G_DEF_INTERFACE (CLUTTER_TYPE_LAYOUT, "Layout", rbclt_c_clutter);

  G_DEF_CLASS (CLUTTER_TYPE_LAYOUT_FLAGS, "Flags", klass);
  G_DEF_CONSTANTS (klass, CLUTTER_TYPE_LAYOUT_FLAGS, "CLUTTER_LAYOUT_");

  rb_define_method (klass, "layout_flags", rbclt_layout_get_layout_flags, 0);
  rb_define_method (klass, "width_for_height", rbclt_layout_width_for_height, 1);
  rb_define_method (klass, "height_for_width", rbclt_layout_height_for_width, 1);
  rb_define_method (klass, "natural_request", rbclt_layout_natural_request, 0);
  rb_define_method (klass, "tune_request", rbclt_layout_tune_request, 2);
}
