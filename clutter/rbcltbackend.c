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

#include <rbgobject.h>
#include <clutter/clutter.h>

#include "rbclutter.h"

static VALUE
rbclt_backend_get_default (VALUE self)
{
  return GOBJ2RVAL (clutter_get_default_backend ());
}

static VALUE
rbclt_backend_set_resolution (VALUE self, VALUE dpi)
{
  ClutterBackend *backend = CLUTTER_BACKEND (RVAL2GOBJ (self));
  clutter_backend_set_resolution (backend, NUM2DBL (dpi));
  return self;
}

static VALUE
rbclt_backend_get_resolution (VALUE self)
{
  ClutterBackend *backend = CLUTTER_BACKEND (RVAL2GOBJ (self));
  return rb_float_new (clutter_backend_get_resolution (backend));
}

static VALUE
rbclt_backend_set_double_click_time (VALUE self, VALUE msec)
{
  ClutterBackend *backend = CLUTTER_BACKEND (RVAL2GOBJ (self));
  clutter_backend_set_double_click_time (backend, NUM2UINT (msec));
  return self;
}

static VALUE
rbclt_backend_get_double_click_time (VALUE self)
{
  ClutterBackend *backend = CLUTTER_BACKEND (RVAL2GOBJ (self));
  return UINT2NUM (clutter_backend_get_double_click_time (backend));
}

static VALUE
rbclt_backend_set_double_click_distance (VALUE self, VALUE distance)
{
  ClutterBackend *backend = CLUTTER_BACKEND (RVAL2GOBJ (self));
  clutter_backend_set_double_click_distance (backend, NUM2UINT (distance));
  return self;
}

static VALUE
rbclt_backend_get_double_click_distance (VALUE self)
{
  ClutterBackend *backend = CLUTTER_BACKEND (RVAL2GOBJ (self));
  return UINT2NUM (clutter_backend_get_double_click_distance (backend));
}

void
rbclt_backend_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BACKEND, "Backend", rbclt_c_clutter);

  rb_define_singleton_method (klass, "default", rbclt_backend_get_default, 0);
  rb_define_method (klass, "set_resolution", rbclt_backend_set_resolution, 1);
  rb_define_method (klass, "resolution", rbclt_backend_get_resolution, 0);
  rb_define_method (klass, "set_double_click_time", rbclt_backend_set_double_click_time, 1);
  rb_define_method (klass, "double_click_time", rbclt_backend_get_double_click_time, 0);
  rb_define_method (klass, "set_double_click_distance", rbclt_backend_set_double_click_distance, 1);
  rb_define_method (klass, "double_click_distance", rbclt_backend_get_double_click_distance, 0);

  G_DEF_SETTERS (klass);
}
