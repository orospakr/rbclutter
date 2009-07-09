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
rbclt_behaviour_apply (int argc, VALUE *argv, VALUE self)
{
  ClutterBehaviour *behaviour = CLUTTER_BEHAVIOUR (RVAL2GOBJ (self));
  int i;

  for (i = 0; i < argc; i++)
    clutter_behaviour_apply (behaviour, RVAL2GOBJ (argv[i]));

  return self;
}

static VALUE
rbclt_behaviour_remove (VALUE self, VALUE actor)
{
  ClutterBehaviour *behaviour = CLUTTER_BEHAVIOUR (RVAL2GOBJ (self));
  clutter_behaviour_remove (behaviour, RVAL2GOBJ (actor));
  return self;
}

static VALUE
rbclt_behaviour_remove_all (VALUE self)
{
  ClutterBehaviour *behaviour = CLUTTER_BEHAVIOUR (RVAL2GOBJ (self));
  clutter_behaviour_remove_all (behaviour);
  return self;
}

static void
rbclt_behaviour_each_actor_callback (ClutterBehaviour *behaviour, ClutterActor *actor, gpointer data)
{
  rb_yield (GOBJ2RVAL (actor));
}

static VALUE
rbclt_behaviour_each (VALUE self)
{
  ClutterBehaviour *behaviour = CLUTTER_BEHAVIOUR (RVAL2GOBJ (self));
  clutter_behaviour_actors_foreach (behaviour, rbclt_behaviour_each_actor_callback, NULL);
  return self;
}

static VALUE
rbclt_behaviour_n_actors (VALUE self)
{
  ClutterBehaviour *behaviour = CLUTTER_BEHAVIOUR (RVAL2GOBJ (self));
  return INT2NUM (clutter_behaviour_get_n_actors (behaviour));
}

static VALUE
rbclt_behaviour_nth_actor (VALUE self, VALUE index)
{
  ClutterBehaviour *behaviour = CLUTTER_BEHAVIOUR (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_behaviour_get_nth_actor (behaviour, NUM2INT (index)));
}

static void
rbclt_behaviour_get_actors_callback (ClutterBehaviour *behavious, ClutterActor *actor, gpointer data)
{
  rb_ary_push ((VALUE) data, GOBJ2RVAL (actor));
}

static VALUE
rbclt_behaviour_actors (VALUE self)
{
  ClutterBehaviour *behaviour = CLUTTER_BEHAVIOUR (RVAL2GOBJ (self));
  VALUE ary = rb_ary_new ();
  clutter_behaviour_actors_foreach (behaviour, rbclt_behaviour_get_actors_callback, (gpointer) ary);
  return ary;
}

static VALUE
rbclt_behaviour_is_applied (VALUE self, VALUE actor)
{
  ClutterBehaviour *behaviour = CLUTTER_BEHAVIOUR (RVAL2GOBJ (self));
  return clutter_behaviour_is_applied (behaviour, RVAL2GOBJ (actor)) ? Qtrue : Qfalse;
}

void
rbclt_behaviour_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BEHAVIOUR, "Behaviour", rbclt_c_clutter);

  rb_define_method (klass, "apply", rbclt_behaviour_apply, -1);
  rb_define_method (klass, "remove", rbclt_behaviour_remove, 1);
  rb_define_method (klass, "remove_all", rbclt_behaviour_remove_all, 0);
  rb_define_method (klass, "each", rbclt_behaviour_each, 0);
  rb_define_method (klass, "n_actors", rbclt_behaviour_n_actors, 0);
  rb_define_method (klass, "[]", rbclt_behaviour_nth_actor, 1);
  rb_define_method (klass, "actors", rbclt_behaviour_actors, 0);
  rb_define_method (klass, "applied?", rbclt_behaviour_is_applied, 1);
}
