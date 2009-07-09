/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2007-2008  Neil Roberts
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
rbclt_behaviour_depth_initialize (VALUE self,
                                  VALUE alpha,
                                  VALUE start_depth,
                                  VALUE end_depth)
{
  ClutterBehaviour *behaviour;

  behaviour = clutter_behaviour_depth_new (RVAL2GOBJ (alpha),
                                           NUM2INT (start_depth),
                                           NUM2INT (end_depth));

  G_INITIALIZE (self, behaviour);

  return Qnil;
}

static VALUE
rbclt_behaviour_depth_set_bounds (VALUE self,
                                  VALUE depth_start,
                                  VALUE depth_end)
{
  ClutterBehaviourDepth *behaviour = CLUTTER_BEHAVIOUR_DEPTH (RVAL2GOBJ (self));
  clutter_behaviour_depth_set_bounds (behaviour,
                                      NUM2INT (depth_start),
                                      NUM2INT (depth_end));
  return self;
}

static VALUE
rbclt_behaviour_depth_get_bounds (VALUE self)
{
  ClutterBehaviourDepth *behaviour = CLUTTER_BEHAVIOUR_DEPTH (RVAL2GOBJ (self));
  gint depth_start, depth_end;

  clutter_behaviour_depth_get_bounds (behaviour, &depth_start, &depth_end);

  return rb_ary_new3 (2, INT2NUM (depth_start), INT2NUM (depth_end));
}

void
rbclt_behaviour_depth_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BEHAVIOUR_DEPTH,
                             "BehaviourDepth", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_behaviour_depth_initialize, 3);
  rb_define_method (klass, "set_bounds", rbclt_behaviour_depth_set_bounds, 2);
  rb_define_method (klass, "get_bounds", rbclt_behaviour_depth_get_bounds, 0);
}
