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
#include <ruby.h>

#include "rbcltcallbackfunc.h"

/* These functions provide a wrapper around a Ruby proc so that the
   proc value can be stored in a global variable that gets marked so
   that it won't get garbage collected until Glib and Clutter are
   finished with it */

struct _RBCLTCallbackFunc
{
  VALUE proc;
};

static ID id_call;

void
rbclt_callback_func_init ()
{
  id_call = rb_intern ("call");
}

RBCLTCallbackFunc *
rbclt_callback_func_new (VALUE proc)
{
  RBCLTCallbackFunc *func = g_slice_new (RBCLTCallbackFunc);

  func->proc = proc;
  rb_gc_register_address (&func->proc);

  return func;
}

void
rbclt_callback_func_destroy (RBCLTCallbackFunc *func)
{
  rb_gc_unregister_address (&func->proc);
  g_slice_free (RBCLTCallbackFunc, func);
}

VALUE
rbclt_callback_func_invoke (RBCLTCallbackFunc *func, int arg_count,
			    const VALUE *argv)
{
  return rb_funcall2 (func->proc, id_call, arg_count, argv);
}
