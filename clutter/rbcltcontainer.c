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
#include <clutter/clutter-container.h>

#include "rbclutter.h"

static void
rbclt_container_mark_callback (ClutterActor *actor, gpointer data)
{
  rbgobj_gc_mark_instance (actor);
}

static void
rbclt_container_mark (void *p)
{
  clutter_container_foreach (CLUTTER_CONTAINER (p),
			     rbclt_container_mark_callback, NULL);
}

static VALUE
rbclt_container_add (int argc, VALUE *argv, VALUE self)
{
  ClutterContainer *container = CLUTTER_CONTAINER (RVAL2GOBJ (self));
  int i;

  for (i = 0; i < argc; i++)
    {
      gpointer actor = RVAL2GOBJ (argv[i]);
      if (!CLUTTER_IS_ACTOR (actor))
	rb_raise (rb_eArgError, "Actor required");
      clutter_container_add_actor (container, CLUTTER_ACTOR (actor));
    }

  return self;
}

static VALUE
rbclt_container_remove (int argc, VALUE *argv, VALUE self)
{
  ClutterContainer *container = CLUTTER_CONTAINER (RVAL2GOBJ (self));
  int i;

  for (i = 0; i < argc; i++)
    {
      gpointer actor = RVAL2GOBJ (argv[i]);
      if (!CLUTTER_IS_ACTOR (actor))
	rb_raise (rb_eArgError, "Actor required");
      clutter_container_remove_actor (container, CLUTTER_ACTOR (actor));
    }

  return self;
}

static void
rbclt_container_children_callback (ClutterActor *actor, gpointer data)
{
  rb_ary_push ((VALUE) data, GOBJ2RVAL (actor));
}

static VALUE
rbclt_container_children (VALUE self)
{
  ClutterContainer *container = CLUTTER_CONTAINER (RVAL2GOBJ (self));
  VALUE ary = rb_ary_new ();

  clutter_container_foreach (container, rbclt_container_children_callback,
			     (gpointer) ary);

  return ary;
}

static void
rbclt_container_each_callback (ClutterActor *actor, gpointer data)
{
  rb_yield (GOBJ2RVAL (actor));
}

static VALUE
rbclt_container_each (VALUE self)
{
  ClutterContainer *container = CLUTTER_CONTAINER (RVAL2GOBJ (self));

  clutter_container_foreach (container, rbclt_container_each_callback, NULL);

  return self;
}

static VALUE
rbclt_container_find_child_by_name (VALUE self, VALUE name)
{
  ClutterContainer *container = CLUTTER_CONTAINER (RVAL2GOBJ (self));
  ClutterActor *child;

  child = clutter_container_find_child_by_name (container,
						StringValuePtr (name));

  if (child == NULL)
    return Qnil;
  else
    return GOBJ2RVAL (child);
}

static VALUE
rbclt_container_raise_child (int argc, VALUE *argv, VALUE self)
{
  ClutterContainer *container = CLUTTER_CONTAINER (RVAL2GOBJ (self));
  VALUE actor, sibling;

  rb_scan_args (argc, argv, "11", &actor, &sibling);

  clutter_container_raise_child (container,
				 RVAL2GOBJ (actor),
				 RVAL2GOBJ (sibling));

  return self;
}

static VALUE
rbclt_container_lower_child (int argc, VALUE *argv, VALUE self)
{
  ClutterContainer *container = CLUTTER_CONTAINER (RVAL2GOBJ (self));
  VALUE actor, sibling;

  rb_scan_args (argc, argv, "11", &actor, &sibling);

  clutter_container_lower_child (container,
				 RVAL2GOBJ (actor),
				 RVAL2GOBJ (sibling));

  return self;
}

static VALUE
rbclt_container_sort_depth_order (VALUE self)
{
  ClutterContainer *container = CLUTTER_CONTAINER (RVAL2GOBJ (self));
  clutter_container_sort_depth_order (container);
  return self;
}

void
rbclt_container_init ()
{
  VALUE klass = G_DEF_INTERFACE2 (CLUTTER_TYPE_CONTAINER, "Container",
				  rbclt_c_clutter, rbclt_container_mark, NULL);

  rb_define_method (klass, "add", rbclt_container_add, -1);
  rb_define_alias (klass, "<<", "add");
  rb_define_method (klass, "remove", rbclt_container_remove, -1);
  rb_define_method (klass, "children", rbclt_container_children, 0);
  rb_define_method (klass, "each", rbclt_container_each, 0);
  rb_define_method (klass, "find_child_by_name",
		    rbclt_container_find_child_by_name, 1);
  rb_define_method (klass, "raise_child", rbclt_container_raise_child, -1);
  rb_define_method (klass, "lower_child", rbclt_container_lower_child, -1);
  rb_define_method (klass, "sort_depth_order",
		    rbclt_container_sort_depth_order, 0);
}
