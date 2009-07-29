/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2008  Neil Roberts
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
rbclt_score_initialize (VALUE self)
{
  G_INITIALIZE (self, clutter_score_new ());

  return Qnil;
}

static VALUE
rbclt_score_each (VALUE self)
{
  ClutterScore *score = (ClutterScore *) RVAL2GOBJ (self);
  GSList *timelines, *node;
  int state = 0;

  timelines = clutter_score_list_timelines (score);

  for (node = timelines; state == 0 && node; node = node->next)
    rb_protect (rb_yield, GOBJ2RVAL (node->data), &state);

  g_slist_free (timelines);

  if (state)
    rb_jump_tag (state);

  return self;
}

static VALUE
rbclt_score_append (int argc, VALUE *argv, VALUE self)
{
  ClutterScore *score = (ClutterScore *) RVAL2GOBJ (self);
  VALUE arg1, arg2, arg3;
  gulong id;

  argc = rb_scan_args (argc, argv, "12", &arg1, &arg2, &arg3);

  if (argc == 1)
    id = clutter_score_append (score, NULL, RVAL2GOBJ (arg1));
  else if (argc == 2)
    id = clutter_score_append (score, RVAL2GOBJ (arg1), RVAL2GOBJ (arg2));
  else
    id = clutter_score_append_at_marker (score, RVAL2GOBJ (arg1),
                                         StringValuePtr (arg2),
                                         RVAL2GOBJ (arg3));

  return UINT2NUM (id);
}

static VALUE
rbclt_score_append_shift (VALUE self, VALUE tl)
{
  ClutterScore *score = (ClutterScore *) RVAL2GOBJ (self);

  clutter_score_append (score, NULL, RVAL2GOBJ (tl));

  return self;
}

static VALUE
rbclt_score_get_timeline (VALUE self, VALUE id)
{
  ClutterScore *score = (ClutterScore *) RVAL2GOBJ (self);

  return GOBJ2RVAL (clutter_score_get_timeline (score, NUM2UINT (id)));
}

static VALUE
rbclt_score_to_a (VALUE self)
{
  ClutterScore *score = (ClutterScore *) RVAL2GOBJ (self);
  GSList *timelines = clutter_score_list_timelines (score);

  return rbgutil_gslist2ary_and_free (timelines);
}

static VALUE
rbclt_score_remove (VALUE self, VALUE id)
{
  ClutterScore *score = (ClutterScore *) RVAL2GOBJ (self);

  clutter_score_remove (score, NUM2UINT (id));

  return self;
}

static VALUE
rbclt_score_remove_all (VALUE self)
{
  ClutterScore *score = (ClutterScore *) RVAL2GOBJ (self);

  clutter_score_remove_all (score);

  return self;
}

static void
rbclt_score_mark (void *p)
{
  GSList *timelines = clutter_score_list_timelines (CLUTTER_SCORE (p));
  GSList *node;

  for (node = timelines; node; node = node->next)
    rbgobj_gc_mark_instance (node->data);

  g_slist_free (timelines);
}

static VALUE
rbclt_score_start (VALUE self)
{
  ClutterScore *score = CLUTTER_SCORE (RVAL2GOBJ (self));

  clutter_score_start (score);

  return self;
}

static VALUE
rbclt_score_stop (VALUE self)
{
  ClutterScore *score = CLUTTER_SCORE (RVAL2GOBJ (self));

  clutter_score_stop (score);

  return self;
}

static VALUE
rbclt_score_pause (VALUE self)
{
  ClutterScore *score = CLUTTER_SCORE (RVAL2GOBJ (self));

  clutter_score_pause (score);

  return self;
}

static VALUE
rbclt_score_rewind (VALUE self)
{
  ClutterScore *score = CLUTTER_SCORE (RVAL2GOBJ (self));

  clutter_score_rewind (score);

  return self;
}

static VALUE
rbclt_score_is_playing (VALUE self)
{
  ClutterScore *score = CLUTTER_SCORE (RVAL2GOBJ (self));

  return clutter_score_is_playing (score) ? Qtrue : Qfalse;
}

void
rbclt_score_init ()
{
  VALUE klass = G_DEF_CLASS2 (CLUTTER_TYPE_SCORE, "Score", rbclt_c_clutter,
                              rbclt_score_mark, NULL);

  rb_define_method (klass, "initialize", rbclt_score_initialize, 0);
  rb_define_method (klass, "each", rbclt_score_each, 0);
  rb_define_method (klass, "append", rbclt_score_append, -1);
  rb_define_method (klass, "<<", rbclt_score_append_shift, 1);
  rb_define_method (klass, "get_timeline", rbclt_score_get_timeline, 1);
  rb_define_alias (klass, "[]", "get_timeline");
  rb_define_method (klass, "to_a", rbclt_score_to_a, 0);
  rb_define_method (klass, "remove", rbclt_score_remove, 1);
  rb_define_method (klass, "remove_all", rbclt_score_remove_all, 0);
  rb_define_method (klass, "start", rbclt_score_start, 0);
  rb_define_method (klass, "stop", rbclt_score_stop, 0);
  rb_define_method (klass, "pause", rbclt_score_pause, 0);
  rb_define_method (klass, "rewind", rbclt_score_rewind, 0);
  rb_define_method (klass, "playing?", rbclt_score_is_playing, 0);
}
