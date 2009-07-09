/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2007-2008 Neil Roberts
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
rbclt_behaviour_rotate_initialize (VALUE self, VALUE alpha,
                                   VALUE axis, VALUE direction,
                                   VALUE angle_start, VALUE angle_end)
{
  ClutterBehaviour *behaviour;

  behaviour = clutter_behaviour_rotate_new
    (RVAL2GOBJ (alpha),
     RVAL2GENUM (axis, CLUTTER_TYPE_ROTATE_AXIS),
     RVAL2GENUM (direction, CLUTTER_TYPE_ROTATE_DIRECTION),
     NUM2DBL (angle_start),
     NUM2DBL (angle_end));

  G_INITIALIZE (self, behaviour);

  return Qnil;
}

static VALUE
rbclt_behaviour_rotate_get_center (VALUE self)
{
  ClutterBehaviourRotate *brotate = CLUTTER_BEHAVIOUR_ROTATE (RVAL2GOBJ (self));
  ClutterVertex vertex;
  int x, y, z;
  clutter_behaviour_rotate_get_center (brotate, &x, &y, &z);
  vertex.x = CLUTTER_UNITS_FROM_INT (x);
  vertex.y = CLUTTER_UNITS_FROM_INT (y);
  vertex.z = CLUTTER_UNITS_FROM_INT (z);
  return BOXED2RVAL (&vertex, CLUTTER_TYPE_VERTEX);
}

static VALUE
rbclt_behaviour_rotate_set_center (int argc, VALUE *argv, VALUE self)
{
  ClutterBehaviourRotate *brotate = CLUTTER_BEHAVIOUR_ROTATE (RVAL2GOBJ (self));
  VALUE x, y, z;

  if (rb_scan_args (argc, argv, "12", &x, &y, &z) == 1)
    {
      ClutterVertex *vertex
        = (ClutterVertex *) RVAL2BOXED (x, CLUTTER_TYPE_VERTEX);

      clutter_behaviour_rotate_set_center (brotate,
                                           CLUTTER_UNITS_TO_INT (vertex->x),
                                           CLUTTER_UNITS_TO_INT (vertex->y),
                                           CLUTTER_UNITS_TO_INT (vertex->z));
    }
  else
    clutter_behaviour_rotate_set_center (brotate,
                                         NUM2INT (x),
                                         NUM2INT (y),
                                         NUM2INT (z));

  return self;
}

static VALUE
rbclt_behaviour_rotate_center_equals (VALUE self, VALUE center)
{
  ClutterBehaviourRotate *brotate = CLUTTER_BEHAVIOUR_ROTATE (RVAL2GOBJ (self));
  ClutterVertex *vertex
    = (ClutterVertex *) RVAL2BOXED (center, CLUTTER_TYPE_VERTEX);

  clutter_behaviour_rotate_set_center (brotate,
                                       CLUTTER_UNITS_TO_INT (vertex->x),
                                       CLUTTER_UNITS_TO_INT (vertex->y),
                                       CLUTTER_UNITS_TO_INT (vertex->z));
  return center;
}

static VALUE
rbclt_behaviour_rotate_set_bounds (VALUE self,
                                   VALUE angle_start,
                                   VALUE angle_end)
{
  ClutterBehaviourRotate *brotate = CLUTTER_BEHAVIOUR_ROTATE (RVAL2GOBJ (self));

  clutter_behaviour_rotate_set_bounds (brotate,
                                       NUM2DBL (angle_start),
                                       NUM2DBL (angle_end));

  return self;
}

static VALUE
rbclt_behaviour_rotate_get_bounds (VALUE self)
{
  ClutterBehaviourRotate *brotate = CLUTTER_BEHAVIOUR_ROTATE (RVAL2GOBJ (self));
  gdouble angle_start, angle_end;

  clutter_behaviour_rotate_get_bounds (brotate,
                                       &angle_start,
                                       &angle_end);

  return rb_ary_new3 (2, rb_float_new (angle_start), rb_float_new (angle_end));
}

void
rbclt_behaviour_rotate_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BEHAVIOUR_ROTATE,
                             "BehaviourRotate", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_behaviour_rotate_initialize, 5);
  rb_define_method (klass, "center", rbclt_behaviour_rotate_get_center, 0);
  rb_define_method (klass, "set_center", rbclt_behaviour_rotate_set_center, -1);
  rb_define_method (klass, "center=", rbclt_behaviour_rotate_center_equals, 1);
  rb_define_method (klass, "set_bounds", rbclt_behaviour_rotate_set_bounds, 2);
  rb_define_method (klass, "get_bounds", rbclt_behaviour_rotate_get_bounds, 0);
}
