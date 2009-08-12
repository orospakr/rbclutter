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
rbclt_path_initialize (int argc, VALUE *argv, VALUE self)
{
  ClutterPath *path;
  if (argc == 1)
    path = clutter_path_new_with_description (StringValuePtr (argv[0]));
  else
    path = clutter_path_new ();

  G_INITIALIZE (self, path);

  return Qnil;
}

static VALUE
rbclt_path_add_move_to(VALUE self, VALUE x, VALUE y)
{
  ClutterPath *path = CLUTTER_PATH (RVAL2GOBJ (self));

  clutter_path_add_move_to(path, NUM2INT (x), NUM2INT (y));
  return Qnil;
}

static VALUE
rbclt_path_add_rel_move_to(VALUE self, VALUE x, VALUE y)
{
  ClutterPath *path = CLUTTER_PATH (RVAL2GOBJ (self));

  clutter_path_add_rel_move_to(path, NUM2INT (x), NUM2INT (y));
  return Qnil;
}

static VALUE
rbclt_path_add_line_to(VALUE self, VALUE x, VALUE y)
{
  ClutterPath *path = CLUTTER_PATH (RVAL2GOBJ (self));

  clutter_path_add_line_to(path, NUM2INT (x), NUM2INT (y));
  return Qnil;
}

static VALUE
rbclt_path_add_rel_line_to(VALUE self, VALUE x, VALUE y)
{
  ClutterPath *path = CLUTTER_PATH (RVAL2GOBJ (self));

  clutter_path_add_rel_line_to(path, NUM2INT (x), NUM2INT (y));
  return Qnil;
}

static VALUE
rbclt_path_add_curve_to(VALUE self, VALUE x_1, VALUE y_1, VALUE x_2, VALUE y_2, VALUE x_3, VALUE y_3)
{
  ClutterPath *path = CLUTTER_PATH (RVAL2GOBJ (self));

  clutter_path_add_curve_to(path, NUM2INT (x_1), NUM2INT (y_1), NUM2INT (x_2), NUM2INT (y_2), NUM2INT (x_3), NUM2INT (y_3));
  return Qnil;
}

static VALUE
rbclt_path_add_rel_curve_to(VALUE self, VALUE x_1, VALUE y_1, VALUE x_2, VALUE y_2, VALUE x_3, VALUE y_3)
{
  ClutterPath *path = CLUTTER_PATH (RVAL2GOBJ (self));

  clutter_path_add_rel_curve_to(path, NUM2INT (x_1), NUM2INT (y_1), NUM2INT (x_2), NUM2INT (y_2), NUM2INT (x_3), NUM2INT (y_3));
  return Qnil;
}

void
rbclt_path_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_PATH, "Path", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_path_initialize, -1);
  rb_define_method (klass, "add_move_to", rbclt_path_add_move_to, 2);
  rb_define_method (klass, "add_rel_move_to", rbclt_path_add_rel_move_to, 2);

  rb_define_method (klass, "add_line_to", rbclt_path_add_line_to, 2);
  rb_define_method (klass, "add_rel_line_to", rbclt_path_add_rel_line_to, 2);

  rb_define_method (klass, "add_curve_to", rbclt_path_add_curve_to, 6);
  rb_define_method (klass, "add_rel_curve_to", rbclt_path_add_rel_curve_to, 6);
}
