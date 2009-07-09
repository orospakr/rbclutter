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

typedef struct _ListChildPropertiesData ListChildPropertiesData;

struct _ListChildPropertiesData
{
  GParamSpec **properties;
  guint n_properties;
};


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

static VALUE
rbclt_container_child_set (int argc, VALUE *argv, VALUE self)
{
  ClutterContainer *container = CLUTTER_CONTAINER (RVAL2GOBJ (self));
  ClutterActor *actor;
  int i;

  if (argc < 2)
    rb_raise (rb_eArgError, "wrong number of arguments (%d for %d)", argc, 2);
  else if ((argc & 1) == 0)
    rb_raise (rb_eArgError, "wrong number of arguments "
              "(prop-value pairs needed)");

  if (!CLUTTER_IS_ACTOR (actor = (ClutterActor *) RVAL2GOBJ (argv[0])))
    rb_raise (rb_eArgError, "actor required");

  for (i = 1; i < argc; i += 2)
    {
      GValue value;
      const char *prop = StringValuePtr (argv[i]);
      GObjectClass *klass
        = G_OBJECT_CLASS (g_type_class_ref (G_TYPE_FROM_INSTANCE (container)));
      GParamSpec *pspec
        = clutter_container_class_find_child_property (klass, prop);

      g_type_class_unref (klass);

      if (pspec == NULL)
        rb_raise (rb_eArgError, "child property not found \"%s\"", prop);

      memset (&value, 0, sizeof (VALUE));
      g_value_init (&value, pspec->value_type);
      rbgobj_rvalue_to_gvalue (argv[i + 1], &value);
      clutter_container_child_set_property (container, actor, prop, &value);
      g_value_unset (&value);
    }

  return self;
}

static VALUE
rbclt_container_child_get (int argc, VALUE *argv, VALUE self)
{
  ClutterContainer *container = CLUTTER_CONTAINER (RVAL2GOBJ (self));
  ClutterActor *actor;
  VALUE ary = rb_ary_new ();
  int i;

  if (argc < 2)
    rb_raise (rb_eArgError, "wrong number of arguments (%d for %d)", argc, 2);

  if (!CLUTTER_IS_ACTOR (actor = (ClutterActor *) RVAL2GOBJ (argv[0])))
    rb_raise (rb_eArgError, "actor required");

  for (i = 1; i < argc; i++)
    {
      GValue value;
      const char *prop = StringValuePtr (argv[i]);
      GObjectClass *klass
        = G_OBJECT_CLASS (g_type_class_ref (G_TYPE_FROM_INSTANCE (container)));
      GParamSpec *pspec
        = clutter_container_class_find_child_property (klass, prop);

      g_type_class_unref (klass);

      if (pspec == NULL)
        rb_raise (rb_eArgError, "child property not found \"%s\"", prop);

      memset (&value, 0, sizeof (VALUE));
      g_value_init (&value, pspec->value_type);
      clutter_container_child_get_property (container, actor, prop, &value);
      rb_ary_push (ary, GVAL2RVAL (&value));
      g_value_unset (&value);
    }

  if (RARRAY_LEN (ary) == 1)
    return RARRAY_PTR (ary)[0];
  else
    return ary;
}

static GObjectClass *
rbclt_container_get_container_class (VALUE rclass)
{
  GObjectClass *ret = NULL;
  const RGObjClassInfo *cinfo = CLASS2CINFO (rclass);

  if (G_TYPE_IS_CLASSED (cinfo->gtype))
    {
      gpointer cptr = g_type_class_ref (cinfo->gtype);

      if (g_type_interface_peek (cptr, CLUTTER_TYPE_CONTAINER))
        ret = cptr;
      else
        g_type_class_unref (cptr);
    }

  if (ret == NULL)
    rb_raise (rb_eArgError, "expected class including Clutter::Container");

  return ret;
}

static VALUE
rbclt_container_get_child_meta (VALUE self, VALUE child)
{
  ClutterContainer *container = CLUTTER_CONTAINER (RVAL2GOBJ (self));

  return GOBJ2RVAL (clutter_container_get_child_meta (container,
                                                      RVAL2GOBJ (child)));
}

static VALUE
rbclt_container_find_child_property (VALUE self, VALUE rclass, VALUE prop_name)
{
  GObjectClass *klass = rbclt_container_get_container_class (rclass);

  GParamSpec *param =
    clutter_container_class_find_child_property (klass,
                                                 StringValuePtr (prop_name));

  g_type_class_unref (klass);

  return GOBJ2RVAL (param);
}

static VALUE
rbclt_container_do_list_child_properties (VALUE data_arg)
{
  ListChildPropertiesData *data = (ListChildPropertiesData *) data_arg;
  int i;
  VALUE ret = rb_ary_new ();

  for (i = 0; i < data->n_properties; i++)
    rb_ary_push (ret, GOBJ2RVAL (data->properties[i]));

  return ret;
}

static VALUE
rbclt_container_free_list_child_properties_data (VALUE data_arg)
{
  g_free (((ListChildPropertiesData *) data_arg)->properties);

  return Qnil;
}

static VALUE
rbclt_container_list_child_properties (VALUE self, VALUE rclass)
{
  GObjectClass *klass = rbclt_container_get_container_class (rclass);
  ListChildPropertiesData data;

  data.properties
    = clutter_container_class_list_child_properties (klass,
                                                     &data.n_properties);

  g_type_class_unref (klass);

  if (data.properties == NULL)
    rb_raise (rb_eArgError, "expected class including Clutter::Container");

  return rb_ensure (rbclt_container_do_list_child_properties, (VALUE) &data,
                    rbclt_container_free_list_child_properties_data,
                    (VALUE) &data);
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
  rb_define_method (klass, "child_set",
                    rbclt_container_child_set, -1);
  rb_define_method (klass, "child_get",
                    rbclt_container_child_get, -1);
  rb_define_method (klass, "get_child_meta",
                    rbclt_container_get_child_meta, 1);
  rb_define_singleton_method (klass, "find_child_property",
                              rbclt_container_find_child_property, 2);
  rb_define_singleton_method (klass, "list_child_properties",
                              rbclt_container_list_child_properties, 1);
}
