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
rbclt_group_initialize (VALUE self)
{
  rbclt_initialize_unowned (self, clutter_group_new ());

  return Qnil;
}

static VALUE
rbclt_group_get_nth_child (VALUE self, VALUE n)
{
  ClutterGroup *group = CLUTTER_GROUP (RVAL2GOBJ (self));
  ClutterActor *child = clutter_group_get_nth_child (group, NUM2UINT (n));
  return child ? GOBJ2RVAL (child) : Qnil;
}

static VALUE
rbclt_group_n_children (VALUE self)
{
  ClutterGroup *group = CLUTTER_GROUP (RVAL2GOBJ (self));
  return INT2NUM (clutter_group_get_n_children (group));
}

static VALUE
rbclt_group_remove_all (VALUE self)
{
  ClutterGroup *group = CLUTTER_GROUP (RVAL2GOBJ (self));
  clutter_group_remove_all (group);
  return self;
}

void
rbclt_group_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_GROUP, "Group", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_group_initialize, 0);
  rb_define_method (klass, "[]", rbclt_group_get_nth_child, 1);
  rb_define_method (klass, "n_children", rbclt_group_n_children, 0);
  rb_define_method (klass, "remove_all", rbclt_group_remove_all, 0);
}
