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
rbclt_stage_initialize (VALUE self)
{
  ClutterActor *stage;

  stage = clutter_stage_new ();

  rbclt_initialize_unowned (self, stage);

  return Qnil;
}

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
rbclt_stage_get_actor_at_pos (VALUE self, VALUE pick_mode, VALUE x, VALUE y)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_stage_get_actor_at_pos (stage,
                                                    RVAL2GENUM (pick_mode, CLUTTER_TYPE_PICK_MODE),
                                                    NUM2INT (x),
                                                    NUM2INT (y)));
}

static VALUE
rbclt_stage_event (VALUE self, VALUE event_arg)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  ClutterEvent *event = (ClutterEvent *) RVAL2BOXED (event_arg,
                                                     CLUTTER_TYPE_EVENT);

  clutter_stage_event (stage, event);

  return self;
}

static VALUE
rbclt_stage_set_perspective (int argc, VALUE *argv, VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));

  if (argc == 1)
    {
      ClutterPerspective *persp
        = (ClutterPerspective *) RVAL2BOXED (argv[0], CLUTTER_TYPE_PERSPECTIVE);
      clutter_stage_set_perspective (stage, persp);
    }
  else if (argc == 4) {
    ClutterPerspective *persp = (ClutterPerspective*) g_malloc(sizeof(ClutterPerspective));
    persp->fovy = NUM2FLOAT (argv[0]);
    persp->aspect = NUM2FLOAT (argv[1]);
    persp->z_near = NUM2FLOAT (argv[2]);
    persp->z_far =  NUM2DBL (argv[3]);
    clutter_stage_set_perspective (stage, persp);
    /* FIXME: g_free(persp); # I can't do this!  It has to be a leak.  Really, support for
       inline perspective values should just go away. */
  }
  else
    rb_raise (rb_eArgError, "wrong number of arguments (%d for 1 or 4)", argc);

  return self;
}

static VALUE
rbclt_stage_read_pixels (VALUE self, VALUE x, VALUE y,
                         VALUE width_arg, VALUE height_arg)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  guchar *pixels;
  VALUE ret;
  gint width, height;

  width = NUM2INT (width_arg);
  height = NUM2INT (height_arg);

  pixels = clutter_stage_read_pixels (stage, NUM2INT (x), NUM2INT (y),
                                      width, height);

  if (pixels == NULL)
    ret = Qnil;
  else
    {
      ret = rb_str_new ((char *) pixels, width * 4 * height);

      g_free (pixels);
    }

  return ret;
}

static VALUE
rbclt_stage_get_fog (VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  ClutterFog fog;

  clutter_stage_get_fogx (stage, &fog);

  return BOXED2RVAL (&fog, CLUTTER_TYPE_FOG);
}

static VALUE
rbclt_stage_set_fog (int argc, VALUE *argv, VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));

  if (argc == 1)
    {
      ClutterFog *fog = (ClutterFog *) RVAL2BOXED (argv[0], CLUTTER_TYPE_FOG);
      clutter_stage_set_fogx (stage, fog);
    }
  else
    {
      VALUE density, z_near, z_far;
      ClutterFog *fog = (ClutterFog*) g_malloc(sizeof(ClutterFog));

      rb_scan_args (argc, argv, "03", &density, &z_near, &z_far);

      fog->z_near = NUM2FLOAT (z_near);
      fog->z_far = NUM2FLOAT (z_far);

      clutter_stage_set_fog (stage, fog);
      /* FIXME: g_free(fog); # argh, once again can't do this. another leak...? */
    }

  return self;
}

static VALUE
rbclt_stage_fog_equals (VALUE self, VALUE fog_arg)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));
  ClutterFog *fog = (ClutterFog *) RVAL2BOXED (fog_arg, CLUTTER_TYPE_FOG);

  clutter_stage_set_fogx (stage, fog);

  return fog_arg;
}

static VALUE
rbclt_stage_get_resolution (VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));

  return rb_float_new (clutter_stage_get_resolution (stage));
}

static VALUE
rbclt_stage_set_key_focus (VALUE self, VALUE actor)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));

  clutter_stage_set_key_focus (stage, RVAL2GOBJ (actor));

  return self;
}

static VALUE
rbclt_stage_get_key_focus (VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));

  return GOBJ2RVAL (clutter_stage_get_key_focus (stage));
}

static VALUE
rbclt_stage_ensure_current (VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));

  clutter_stage_ensure_current (stage);

  return self;
}

static VALUE
rbclt_stage_is_default (VALUE self)
{
  ClutterStage *stage = CLUTTER_STAGE (RVAL2GOBJ (self));

  return clutter_stage_is_default (stage) ? Qtrue : Qfalse;
}

void
rbclt_stage_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_STAGE, "Stage", rbclt_c_clutter);

  rb_define_singleton_method (klass, "get_default", rbclt_stage_get_default, 0);

  rb_define_method (klass, "initialize", rbclt_stage_initialize, 0);
  rb_define_method (klass, "fullscreen", rbclt_stage_fullscreen, 0);
  rb_define_method (klass, "unfullscreen", rbclt_stage_unfullscreen, 0);
  rb_define_method (klass, "show_cursor", rbclt_stage_show_cursor, 0);
  rb_define_method (klass, "hide_cursor", rbclt_stage_hide_cursor, 0);
  rb_define_method (klass, "get_actor_at_pos", rbclt_stage_get_actor_at_pos, 2);
  rb_define_method (klass, "event", rbclt_stage_event, 1);
  rb_define_method (klass, "set_perspective", rbclt_stage_set_perspective, -1);
  rb_define_method (klass, "read_pixels", rbclt_stage_read_pixels, 4);
  rb_define_method (klass, "fog", rbclt_stage_get_fog, 0);
  rb_define_method (klass, "set_fog", rbclt_stage_set_fog, -1);
  rb_define_method (klass, "fog=", rbclt_stage_fog_equals, 1);
  rb_define_method (klass, "resolution", rbclt_stage_get_resolution, 0);
  rb_define_method (klass, "set_key_focus", rbclt_stage_set_key_focus, 1);
  rb_define_method (klass, "key_focus", rbclt_stage_get_key_focus, 0);
  rb_define_method (klass, "ensure_current", rbclt_stage_ensure_current, 0);
  rb_define_method (klass, "default?", rbclt_stage_is_default, 0);

  G_DEF_SETTERS (klass);
}
