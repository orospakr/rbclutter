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
#include <clutter-gst/clutter-gst-video-sink.h>

#include "rbcluttergst.h"

static VALUE
rbcltgst_video_sink_initialize (VALUE self, VALUE texture)
{
  GstElement *elem = clutter_gst_video_sink_new (RVAL2GOBJ (texture));
  rbcltgst_initialize_gst_object (self, elem);
  return Qnil;
}

void
rbcltgst_video_sink_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_GST_TYPE_VIDEO_SINK, "VideoSink", rbcltgst_c_clutter_gst);

  rb_define_method (klass, "initialize", rbcltgst_video_sink_initialize, 1);
}
