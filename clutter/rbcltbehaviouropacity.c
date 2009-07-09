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
rbclt_behaviour_opacity_initialize (VALUE self, VALUE alpha,
                                    VALUE opacity_start, VALUE opacity_end)
{
  ClutterBehaviour *behaviour;

  behaviour = clutter_behaviour_opacity_new
    (RVAL2GOBJ (alpha), rbclt_num_to_guint8 (opacity_start),
     rbclt_num_to_guint8 (opacity_end));

  G_INITIALIZE (self, behaviour);

  return Qnil;
}

static VALUE
rbclt_behaviour_opacity_set_bounds (VALUE self,
                                    VALUE opacity_start,
                                    VALUE opacity_end)
{
  ClutterBehaviourOpacity *behaviour
    = CLUTTER_BEHAVIOUR_OPACITY (RVAL2GOBJ (self));

  clutter_behaviour_opacity_set_bounds (behaviour,
                                        rbclt_num_to_guint8 (opacity_start),
                                        rbclt_num_to_guint8 (opacity_end));

  return self;
}

static VALUE
rbclt_behaviour_opacity_get_bounds (VALUE self)
{
  ClutterBehaviourOpacity *behaviour
    = CLUTTER_BEHAVIOUR_OPACITY (RVAL2GOBJ (self));
  guint8 opacity_start, opacity_end;

  clutter_behaviour_opacity_get_bounds (behaviour,
                                        &opacity_start, &opacity_end);

  return rb_ary_new3 (2, INT2NUM (opacity_start), INT2NUM (opacity_end));
}

void
rbclt_behaviour_opacity_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BEHAVIOUR_OPACITY,
                             "BehaviourOpacity", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_behaviour_opacity_initialize, 3);
  rb_define_method (klass, "set_bounds", rbclt_behaviour_opacity_set_bounds, 2);
  rb_define_method (klass, "get_bounds", rbclt_behaviour_opacity_get_bounds, 0);
}
