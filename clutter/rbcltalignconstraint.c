/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2007-2008  Neil Roberts
 * Copyright (C) 2011       Andrew Clunis <andrew@orospakr.ca>
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

/* http://docs.clutter-project.org/docs/clutter/stable/ClutterAlignConstraint.html */

static VALUE
rbclt_align_constraint_initialize (VALUE self, VALUE source, VALUE axis, VALUE factor)
{
  ClutterActor *g_source = CLUTTER_ACTOR (RVAL2GOBJ (source));
  rbclt_initialize_unowned (self, clutter_align_constraint_new (g_source, RVAL2GENUM (axis, CLUTTER_TYPE_ALIGN_AXIS), NUM2DBL (factor)));
  return Qnil;
}

void
rbclt_align_constraint_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_ALIGN_CONSTRAINT, "AlignConstraint", rbclt_c_clutter);
  
  rb_define_method (klass, "initialize", rbclt_align_constraint_initialize, 3);
  // rb_define_method (klass, "set_source", rbclt_align_constraint_set_source, 1);
}
