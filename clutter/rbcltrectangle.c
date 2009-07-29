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
rbclt_rectangle_initialize (int argc, VALUE *argv, VALUE self)
{
  VALUE color;
  ClutterActor *actor;

  rb_scan_args (argc, argv, "01", &color);

  actor = color == Qnil ? clutter_rectangle_new ()
    : clutter_rectangle_new_with_color ((ClutterColor *) RVAL2BOXED (color, CLUTTER_TYPE_COLOR));

  rbclt_initialize_unowned (self, actor);

  return Qnil;
}

void
rbclt_rectangle_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_RECTANGLE, "Rectangle", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_rectangle_initialize, -1);

  G_DEF_SETTERS (klass);
}
