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
#include <clutter/clutter-timeline.h>

#include "rbclutter.h"

static VALUE
rbclt_timeline_initialize (VALUE self, VALUE n_frame, VALUE fps)
{
  G_INITIALIZE (self, clutter_timeline_new (NUM2UINT (n_frame), NUM2UINT (fps)));

  return Qnil;
}

static VALUE
rbclt_timeline_dup (VALUE self)
{
  ClutterTimeline *timeline = CLUTTER_TIMELINE (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_timeline_clone (timeline));
}

static VALUE
rbclt_timeline_start (VALUE self)
{
  ClutterTimeline *timeline = CLUTTER_TIMELINE (RVAL2GOBJ (self));
  clutter_timeline_start (timeline);
  return self;
}

static VALUE
rbclt_timeline_pause (VALUE self)
{
  ClutterTimeline *timeline = CLUTTER_TIMELINE (RVAL2GOBJ (self));
  clutter_timeline_pause (timeline);
  return self;
}

static VALUE
rbclt_timeline_stop (VALUE self)
{
  ClutterTimeline *timeline = CLUTTER_TIMELINE (RVAL2GOBJ (self));
  clutter_timeline_stop (timeline);
  return self;
}

static VALUE
rbclt_timeline_rewind (VALUE self)
{
  ClutterTimeline *timeline = CLUTTER_TIMELINE (RVAL2GOBJ (self));
  clutter_timeline_rewind (timeline);
  return self;
}

static VALUE
rbclt_timeline_skip (VALUE self, VALUE n_frames)
{
  ClutterTimeline *timeline = CLUTTER_TIMELINE (RVAL2GOBJ (self));
  clutter_timeline_skip (timeline, NUM2UINT (n_frames));
  return self;
}

static VALUE
rbclt_timeline_advance (VALUE self, VALUE frame_num)
{
  ClutterTimeline *timeline = CLUTTER_TIMELINE (RVAL2GOBJ (self));
  clutter_timeline_advance (timeline, NUM2UINT (frame_num));
  return self;
}

static VALUE
rbclt_timeline_current_frame (VALUE self)
{
  ClutterTimeline *timeline = CLUTTER_TIMELINE (RVAL2GOBJ (self));
  return INT2NUM (clutter_timeline_get_current_frame (timeline));
}

static VALUE
rbclt_timeline_is_playing (VALUE self)
{
  ClutterTimeline *timeline = CLUTTER_TIMELINE (RVAL2GOBJ (self));
  return clutter_timeline_is_playing (timeline) ? Qtrue : Qfalse;
}

void
rbclt_timeline_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_TIMELINE, "Timeline", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_timeline_initialize, 2);
  rb_define_method (klass, "dup", rbclt_timeline_dup, 0);
  rb_define_alias (klass, "speed", "fps");
  rb_define_alias (klass, "speed=", "fps=");
  rb_define_alias (klass, "set_speed", "set_fps");
  rb_define_method (klass, "start", rbclt_timeline_start, 0);
  rb_define_method (klass, "pause", rbclt_timeline_pause, 0);
  rb_define_method (klass, "stop", rbclt_timeline_stop, 0);
  rb_define_method (klass, "rewind", rbclt_timeline_rewind, 0);
  rb_define_method (klass, "skip", rbclt_timeline_skip, 1);
  rb_define_method (klass, "advance", rbclt_timeline_advance, 1);
  rb_define_method (klass, "current_frame", rbclt_timeline_current_frame, 0);
  rb_define_method (klass, "playing?", rbclt_timeline_is_playing, 0);
  
  G_DEF_SETTERS (klass);
}
