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
rbclt_behaviour_ellipse_initialize (VALUE self, VALUE alpha,
                                    VALUE x, VALUE y,
                                    VALUE width, VALUE height,
                                    VALUE direction,
                                    VALUE start, VALUE end)
{
  ClutterBehaviour *behaviour;

  behaviour = clutter_behaviour_ellipse_new (RVAL2GOBJ (alpha),
                                             NUM2INT (x), NUM2INT (y),
                                             NUM2INT (width), NUM2INT (height),
                                             RVAL2GENUM (direction, CLUTTER_TYPE_ROTATE_DIRECTION),
                                             NUM2DBL (start), NUM2DBL (end));

  G_INITIALIZE (self, behaviour);

  return Qnil;
}

static VALUE
rbclt_behaviour_ellipse_set_center (int argc, VALUE *argv, VALUE self)
{
  ClutterBehaviourEllipse *bellipse = CLUTTER_BEHAVIOUR_ELLIPSE (RVAL2GOBJ (self));
  VALUE x, y;

  rb_scan_args (argc, argv, "11", &x, &y);

  if (y == Qnil)
    g_object_set (bellipse, "center", RVAL2BOXED (x, CLUTTER_TYPE_KNOT), NULL);
  else
    clutter_behaviour_ellipse_set_center (bellipse, NUM2INT (x), NUM2INT (y));

  return self;
}

static VALUE
rbclt_behaviour_ellipse_set_tilt (VALUE self, VALUE angle_tilt_x,
                                  VALUE angle_tilt_y, VALUE angle_tilt_z)
{
  ClutterBehaviourEllipse *bellipse = CLUTTER_BEHAVIOUR_ELLIPSE (RVAL2GOBJ (self));
  clutter_behaviour_ellipse_set_tilt (bellipse, NUM2DBL (angle_tilt_x), NUM2DBL (angle_tilt_y),
                                      NUM2DBL (angle_tilt_z));
  return self;
}

void
rbclt_behaviour_ellipse_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BEHAVIOUR_ELLIPSE, "BehaviourEllipse", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_behaviour_ellipse_initialize, 8);
  rb_define_method (klass, "set_center", rbclt_behaviour_ellipse_set_center, -1);
  rb_define_method (klass, "set_tilt", rbclt_behaviour_ellipse_set_tilt, 3);
}
