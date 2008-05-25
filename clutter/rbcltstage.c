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
#include <clutter/clutter-stage.h>

#include "rbclutter.h"

static VALUE
rbclt_stage_get_default ()
{
  ClutterActor *actor = clutter_stage_get_default ();

  if (actor == NULL)
    return Qnil;
  else
    return GOBJ2RVAL (actor);
}

static VALUE
rbclt_stage_fullscreen (VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  clutter_stage_fullscreen (stage);
  return self;
}

static VALUE
rbclt_stage_unfullscreen (VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  clutter_stage_unfullscreen (stage);
  return self;
}

static VALUE
rbclt_stage_show_cursor (VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  clutter_stage_show_cursor (stage);
  return self;
}

static VALUE
rbclt_stage_hide_cursor (VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  clutter_stage_hide_cursor (stage);
  return self;
}

static VALUE
rbclt_stage_get_actor_at_pos (VALUE self, VALUE x, VALUE y)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_stage_get_actor_at_pos (stage, NUM2INT (x), NUM2INT (y)));
}

static VALUE
rbclt_stage_snapshot (int argc, VALUE *argv, VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  VALUE x_arg, y_arg, width_arg, height_arg;
  gint x, y, width, height;

  rb_scan_args (argc, argv, "04", &x_arg, &y_arg, &width_arg, &height_arg);

  x = x_arg == Qnil ? 0 : NUM2INT (x_arg);
  y = y_arg == Qnil ? 0 : NUM2INT (y_arg);
  width = width_arg == Qnil ? clutter_actor_get_width (CLUTTER_ACTOR (stage)) - x : NUM2INT (width_arg);
  height = height_arg == Qnil ? clutter_actor_get_height (CLUTTER_ACTOR (stage)) - y : NUM2INT (height_arg);

  return GOBJ2RVAL (clutter_stage_snapshot (stage, x, y, width, height));
}

static VALUE
rbclt_stage_event (VALUE self, VALUE event)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  clutter_stage_event (stage, (ClutterEvent *) RVAL2BOXED (event, CLUTTER_TYPE_EVENT));
  return self;
}

static VALUE
rbclt_stage_set_user_resizable (VALUE self, VALUE val)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  clutter_stage_set_user_resizable (stage, RTEST (val) ? TRUE : FALSE);
  return self;
}

static VALUE
rbclt_stage_get_user_resizable (VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  return clutter_stage_get_user_resizable (stage) ? Qtrue : Qfalse;
}

static VALUE
rbclt_stage_get_perspective (VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  ClutterPerspective perspective;
  clutter_stage_get_perspectivex (stage, &perspective);
  return BOXED2RVAL (&perspective, CLUTTER_TYPE_PERSPECTIVE);
}

static VALUE
rbclt_stage_set_perspective (int argc, VALUE *argv, VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));

  if (argc == 1)
    clutter_stage_set_perspectivex (stage, RVAL2BOXED (argv[0], CLUTTER_TYPE_PERSPECTIVE));
  else if (argc == 4)
    clutter_stage_set_perspective (stage, NUM2DBL (argv[0]), NUM2DBL (argv[1]),
				   NUM2DBL (argv[2]), NUM2DBL (argv[3]));
  else
    rb_raise (rb_eArgError, "wrong number of arguments (%d for 1 or 4)", argc);

  return self;
}

static VALUE
rbclt_stage_perspective_equals (VALUE self, VALUE value)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  clutter_stage_set_perspectivex (stage, RVAL2BOXED (value, CLUTTER_TYPE_PERSPECTIVE));
  return value;
}

void
rbclt_stage_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_STAGE, "Stage", rbclt_c_clutter);

  rb_define_singleton_method (klass, "get_default", rbclt_stage_get_default, 0);

  rb_define_method (klass, "fullscreen", rbclt_stage_fullscreen, 0);
  rb_define_method (klass, "unfullscreen", rbclt_stage_unfullscreen, 0);
  rb_define_method (klass, "show_cursor", rbclt_stage_show_cursor, 0);
  rb_define_method (klass, "hide_cursor", rbclt_stage_hide_cursor, 0);
  rb_define_method (klass, "get_actor_at_pos", rbclt_stage_get_actor_at_pos, 2);
  rb_define_method (klass, "snapshot", rbclt_stage_snapshot, -1);
  rb_define_method (klass, "event", rbclt_stage_event, 1);
  rb_define_method (klass, "set_user_resizable", rbclt_stage_set_user_resizable, 0);
  rb_define_method (klass, "user_resizable?", rbclt_stage_get_user_resizable, 0);
  rb_define_method (klass, "perspective", rbclt_stage_get_perspective, 0);
  rb_define_method (klass, "set_perspective", rbclt_stage_set_perspective, -1);
  rb_define_method (klass, "perspective=", rbclt_stage_perspective_equals, 1);

  G_DEF_SETTERS (klass);
}
