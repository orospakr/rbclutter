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

#include <clutter/clutter.h>
#include <rbgobject.h>

#include "rbclutter.h"
#include "rbcltcallbackfunc.h"

static gboolean
rbclt_frame_source_callback (gpointer data)
{
  return RTEST (rbclt_callback_func_invoke ((RBCLTCallbackFunc *) data,
                                            0, NULL));
}

static VALUE
rbclt_frame_source_add (int argc, VALUE *argv, VALUE self)
{
  VALUE priority_arg, interval_arg, func;
  gint priority;
  guint id, interval;
  RBCLTCallbackFunc *func_wrapper;

  rb_scan_args (argc, argv, "11&", &interval_arg, &priority_arg, &func);

  priority = NIL_P (priority_arg) ? G_PRIORITY_DEFAULT : NUM2INT (priority_arg);
  interval = NUM2UINT (interval_arg);
  func_wrapper = rbclt_callback_func_new (func);

  id = clutter_frame_source_add_full
    (priority, interval, rbclt_frame_source_callback, func_wrapper,
     (GDestroyNotify) rbclt_callback_func_destroy);

  return UINT2NUM (id);
}

void
rbclt_frame_source_init ()
{
  VALUE klass;

  klass = rb_define_module_under (rbclt_c_clutter, "FrameSource");

  rb_define_module_function (klass, "add", rbclt_frame_source_add, -1);
}
