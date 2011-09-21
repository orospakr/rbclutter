/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2007-2008  Neil Roberts
 * Copyright (C) 2011       Andrew Clunis
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

/* http://docs.clutter-project.org/docs/clutter/stable/ClutterFlowLayout.html */

static VALUE
rbclt_flow_layout_initialize (VALUE self, VALUE orientation)
{
  ClutterGravity orientation_val = RVAL2GENUM (orientation, CLUTTER_TYPE_FLOW_ORIENTATION);
  rbclt_initialize_unowned (self, clutter_flow_layout_new (orientation_val));

  return Qnil;
}

void
rbclt_flow_layout_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_FLOW_LAYOUT, "FlowLayout", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_flow_layout_initialize, 1);
}
