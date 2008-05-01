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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <rbgobject.h>
#include <clutter/clutter-actor.h>
#include <clutter/clutter-behaviour.h>
#include <clutter/clutter-behaviour-bspline.h>

#include "rbclutter.h"

static VALUE
rbclt_behaviour_bspline_initialize (int argc, VALUE *argv, VALUE self)
{
  ClutterBehaviour *behaviour = clutter_behaviour_bspline_new (NULL, 0, 0);
  int i;

  G_INITIALIZE (self, behaviour);

  if (argc >= 1)
    clutter_behaviour_set_alpha (behaviour, RVAL2GOBJ (argv[0]));
  for (i = 1; i < argc; i++)
    clutter_behaviour_bspline_append_knot (CLUTTER_BEHAVIOUR_BSPLINE (behaviour),
					   RVAL2BOXED (argv[i], CLUTTER_TYPE_KNOT));

  return Qnil;
}

static VALUE
rbclt_behaviour_bspline_append_knot (int argc, VALUE *argv, VALUE self)
{
  ClutterBehaviourBspline *bbspline = CLUTTER_BEHAVIOUR_BSPLINE (RVAL2GOBJ (self));
  int i;
  
  for (i = 0; i < argc; i++)
    clutter_behaviour_bspline_append_knot (bbspline, RVAL2BOXED (argv[i], CLUTTER_TYPE_KNOT));

  return self;
}

static VALUE
rbclt_behaviour_bspline_clear (VALUE self)
{
  ClutterBehaviourBspline *bbspline = CLUTTER_BEHAVIOUR_BSPLINE (RVAL2GOBJ (self));
  clutter_behaviour_bspline_clear (bbspline);
  return self;
}

static VALUE
rbclt_behaviour_bspline_truncate (VALUE self, VALUE offset)
{
  ClutterBehaviourBspline *bbspline = CLUTTER_BEHAVIOUR_BSPLINE (RVAL2GOBJ (self));
  clutter_behaviour_bspline_truncate (bbspline, NUM2UINT (offset));
  return self;
}

static VALUE
rbclt_behaviour_bspline_join (VALUE self, VALUE other)
{
  ClutterBehaviourBspline *bbspline = CLUTTER_BEHAVIOUR_BSPLINE (RVAL2GOBJ (self));
  clutter_behaviour_bspline_join (bbspline, RVAL2GOBJ (other));
  return self;
}

static VALUE
rbclt_behaviour_bspline_split (VALUE self, VALUE offset)
{
  ClutterBehaviourBspline *bbspline = CLUTTER_BEHAVIOUR_BSPLINE (RVAL2GOBJ (self));
  ClutterBehaviour *other;
  VALUE ret;

  other = clutter_behaviour_bspline_split (bbspline, NUM2UINT (offset));
  if (other == NULL)
    ret = Qnil;
  else
    {
      ret = GOBJ2RVAL (other);
      g_object_unref (other);
    }

  return ret;
}

static VALUE
rbclt_behaviour_bspline_adjust (VALUE self, VALUE offset, VALUE knot)
{
  ClutterBehaviourBspline *bbspline = CLUTTER_BEHAVIOUR_BSPLINE (RVAL2GOBJ (self));
  clutter_behaviour_bspline_adjust (bbspline, NUM2UINT (offset),
				    (ClutterKnot *) RVAL2BOXED (knot, CLUTTER_TYPE_KNOT));
  return self;
}

static VALUE
rbclt_behaviour_bspline_set_origin (VALUE self, VALUE knot)
{
  ClutterBehaviourBspline *bbspline = CLUTTER_BEHAVIOUR_BSPLINE (RVAL2GOBJ (self));
  clutter_behaviour_bspline_set_origin (bbspline, 
					(ClutterKnot *) RVAL2BOXED (knot, CLUTTER_TYPE_KNOT));
  return self;
}

static VALUE
rbclt_behaviour_bspline_get_origin (VALUE self)
{
  ClutterBehaviourBspline *bbspline = CLUTTER_BEHAVIOUR_BSPLINE (RVAL2GOBJ (self));
  ClutterKnot knot;
  clutter_behaviour_bspline_get_origin (bbspline, &knot);
  return BOXED2RVAL (&knot, CLUTTER_TYPE_KNOT);
}

void
rbclt_behaviour_bspline_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BEHAVIOUR_BSPLINE, "BehaviourBspline", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_behaviour_bspline_initialize, -1);
  rb_define_method (klass, "append_knot", rbclt_behaviour_bspline_append_knot, -1);
  rb_define_alias (klass, "append_knots", "append_knot");
  rb_define_alias (klass, "append", "append_knot");
  rb_define_alias (klass, "<<", "append_knot");
  rb_define_method (klass, "clear", rbclt_behaviour_bspline_clear, 0);
  rb_define_method (klass, "truncate", rbclt_behaviour_bspline_truncate, 1);
  rb_define_method (klass, "join", rbclt_behaviour_bspline_join, 1);
  rb_define_method (klass, "split", rbclt_behaviour_bspline_split, 1);
  rb_define_method (klass, "adjust", rbclt_behaviour_bspline_adjust, 2);
  rb_define_method (klass, "set_origin", rbclt_behaviour_bspline_set_origin, 1);
  rb_define_method (klass, "origin", rbclt_behaviour_bspline_get_origin, 0);

  G_DEF_SETTERS (klass);
}
