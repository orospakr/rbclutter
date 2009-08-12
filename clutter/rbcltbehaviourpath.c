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
rbclt_behaviour_path_initialize (VALUE self, VALUE alpha, VALUE path)
{
  ClutterAlpha *c_alpha = CLUTTER_ALPHA (RVAL2GOBJ (alpha));
  ClutterPath *c_path = CLUTTER_PATH (RVAL2GOBJ (path));

  /* increase the refcount by 1, because even though path is a GInitiallyUnowned,
     ruby-gnome2 is still unref'ing it when it goes out of scope in Ruby.

     However, I may have the GObject floating reference semantics a bit wrong
     and this might be leaky.  Way better than crashing, though...
  */
  g_object_ref (c_path);

  ClutterBehaviour *behaviour = clutter_behaviour_path_new (c_alpha, c_path);

  G_INITIALIZE (self, behaviour);

  return Qnil;
}

static VALUE
rbclt_behaviour_path_knots (VALUE self)
{
  ClutterBehaviourPath *bpath = CLUTTER_BEHAVIOUR_PATH (RVAL2GOBJ (self));
  return GSLIST2ARY2F (clutter_behaviour_path_get_knots (bpath), CLUTTER_TYPE_KNOT);
}

static VALUE
rbclt_behaviour_path_append_knot (int argc, VALUE *argv, VALUE self)
{
  ClutterBehaviourPath *bpath = CLUTTER_BEHAVIOUR_PATH (RVAL2GOBJ (self));
  int i;

  for (i = 0; i < argc; i++)
    clutter_behaviour_path_append_knot (bpath, RVAL2BOXED (argv[i], CLUTTER_TYPE_KNOT));

  return self;
}

static VALUE
rbclt_behaviour_path_insert_knot (VALUE self, VALUE offset, VALUE knot)
{
  ClutterBehaviourPath *bpath = CLUTTER_BEHAVIOUR_PATH (RVAL2GOBJ (self));
  clutter_behaviour_path_insert_knot (bpath, NUM2UINT (offset),
                                      RVAL2BOXED (knot, CLUTTER_TYPE_KNOT));
  return self;
}

static VALUE
rbclt_behaviour_path_remove_knot (VALUE self, VALUE offset)
{
  ClutterBehaviourPath *bpath = CLUTTER_BEHAVIOUR_PATH (RVAL2GOBJ (self));
  clutter_behaviour_path_remove_knot (bpath, NUM2UINT (offset));
  return self;
}

static VALUE
rbclt_behaviour_path_clear (VALUE self)
{
  ClutterBehaviourPath *bpath = CLUTTER_BEHAVIOUR_PATH (RVAL2GOBJ (self));
  clutter_behaviour_path_clear (bpath);
  return self;
}

void
rbclt_behaviour_path_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BEHAVIOUR_PATH, "BehaviourPath", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_behaviour_path_initialize, 2);
  rb_define_method (klass, "knots", rbclt_behaviour_path_knots, 0);
  rb_define_method (klass, "append_knot", rbclt_behaviour_path_append_knot, -1);
  rb_define_alias (klass, "append_knots", "append_knot");
  rb_define_alias (klass, "append", "append_knot");
  rb_define_alias (klass, "<<", "append_knot");
  rb_define_method (klass, "insert_knot", rbclt_behaviour_path_insert_knot, 2);
  rb_define_method (klass, "remove_knot", rbclt_behaviour_path_remove_knot, 1);
  rb_define_method (klass, "clear", rbclt_behaviour_path_clear, 0);
}
