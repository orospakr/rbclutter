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
rbclt_behaviour_scale_initialize (VALUE self, VALUE alpha,
                                  VALUE x_scale_start, VALUE y_scale_start,
                                  VALUE x_scale_end, VALUE y_scale_end)
{
  ClutterBehaviour *behaviour;

  behaviour = clutter_behaviour_scale_new (RVAL2GOBJ (alpha),
                                           NUM2DBL (x_scale_start),
                                           NUM2DBL (y_scale_start),
                                           NUM2DBL (x_scale_end),
                                           NUM2DBL (y_scale_end));

  G_INITIALIZE (self, behaviour);

  return Qnil;
}

static VALUE
rbclt_behaviour_scale_set_bounds (VALUE self,
                                  VALUE x_scale_start, VALUE y_scale_start,
                                  VALUE x_scale_end, VALUE y_scale_end)
{
  ClutterBehaviourScale *behaviour = CLUTTER_BEHAVIOUR_SCALE (RVAL2GOBJ (self));

  clutter_behaviour_scale_set_bounds (behaviour,
                                      NUM2DBL (x_scale_start),
                                      NUM2DBL (y_scale_start),
                                      NUM2DBL (x_scale_end),
                                      NUM2DBL (y_scale_end));

  return self;
}

static VALUE
rbclt_behaviour_scale_get_bounds (VALUE self)
{
  ClutterBehaviourScale *behaviour = CLUTTER_BEHAVIOUR_SCALE (RVAL2GOBJ (self));
  gdouble x_scale_start, y_scale_start;
  gdouble x_scale_end, y_scale_end;

  clutter_behaviour_scale_get_bounds (behaviour,
                                      &x_scale_start,
                                      &y_scale_start,
                                      &x_scale_end,
                                      &y_scale_end);

  return rb_ary_new3 (4, rb_float_new (x_scale_start),
                      rb_float_new (y_scale_start),
                      rb_float_new (x_scale_end),
                      rb_float_new (y_scale_end));
}

void
rbclt_behaviour_scale_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BEHAVIOUR_SCALE, "BehaviourScale",
                             rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_behaviour_scale_initialize, 5);
  rb_define_method (klass, "set_bounds", rbclt_behaviour_scale_set_bounds, 4);
  rb_define_method (klass, "get_bounds", rbclt_behaviour_scale_get_bounds, 0);
}
