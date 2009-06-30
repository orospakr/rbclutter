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

#include <ruby.h>
#include <rbgobject.h>
#include <gst/gstobject.h>
#include <clutter-gst/clutter-gst-util.h>
#include <clutter-gst/clutter-gst-version.h>
#include <clutter/clutter-enum-types.h>
#include <rbclutter.h>

#include "rbcluttergst.h"

VALUE rbcltgst_c_clutter_gst = Qnil;

extern void rbcltgst_video_sink_init ();
extern void rbcltgst_video_texture_init ();
extern void rbcltgst_audio_init ();

void
rbcltgst_initialize_gst_object (VALUE obj, gpointer gstobj)
{
  /* Grab the floating reference if the object is a subclass of
     GstObject */
  if (GST_IS_OBJECT (gstobj))
    {
      gst_object_ref (gstobj);
      gst_object_sink (gstobj);
    }

  G_INITIALIZE (obj, gstobj);
}

void
Init_clutter_gst ()
{
  rb_require ("glib2");
  rb_require ("clutter");
  rb_require ("gst");

  /* The base class init of some of the clutter-gst classes call some
     gst functions so unfortunatly defining the Ruby wrappers will
     crash if clutter_gst_init is not called first */
  rbclt_call_init_func (0, NULL, clutter_gst_init);

  rbcltgst_c_clutter_gst = rb_define_module_under (rbclt_c_clutter, "Gst");

  /* Version constants. Not defining a regular VERSION constant
     because under Ruby-GTK that constant refers to the version of the
     actual library binary, but there isn't a way to get that with
     Clutter yet. */
  rb_define_const (rbcltgst_c_clutter_gst, "BUILD_VERSION",
                   rb_ary_new3 (3, INT2FIX (CLUTTER_GST_MAJOR_VERSION),
                                INT2FIX (CLUTTER_GST_MINOR_VERSION),
                                INT2FIX (CLUTTER_GST_MICRO_VERSION)));

  rbcltgst_video_sink_init ();
  rbcltgst_video_texture_init ();
  rbcltgst_audio_init ();
}
