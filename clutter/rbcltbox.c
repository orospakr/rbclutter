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

/* http://docs.clutter-project.org/docs/clutter/stable/ClutterBox.html */

static VALUE
rbclt_box_initialize (VALUE self, VALUE layout_manager)
{
  ClutterLayoutManager *layout_manager_g = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (layout_manager));
  rbclt_initialize_unowned (self, clutter_box_new (layout_manager_g));
  return Qnil;
}

static VALUE
rbclt_box_set_layout_manager (VALUE self, VALUE layout_manager)
{
  ClutterBox *box = CLUTTER_BOX (RVAL2GOBJ (self));
  ClutterLayoutManager *layout_manager_g = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (layout_manager));
  clutter_box_set_layout_manager(box, layout_manager_g);
  return Qnil;
}

static VALUE
rbclt_box_get_layout_manager (VALUE self)
{
  ClutterBox *box = CLUTTER_BOX (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_box_get_layout_manager (box));
}

static VALUE
rbclt_box_set_color (VALUE self, VALUE color)
{
  ClutterBox *box = CLUTTER_BOX (RVAL2GOBJ (self));
  clutter_box_set_color(box, (ClutterColor *) RVAL2BOXED (color, CLUTTER_TYPE_COLOR));

  return Qnil;
}

static VALUE
rbclt_box_get_color (VALUE self)
{
  ClutterBox *box = CLUTTER_BOX (RVAL2GOBJ (self));
  ClutterColor color;

  clutter_box_get_color (box, &color);
  return BOXED2RVAL (&color, CLUTTER_TYPE_COLOR);
}

static VALUE
rbclt_box_pack (VALUE self, VALUE actor, VALUE properties)
{
  ClutterBox *box = CLUTTER_BOX (RVAL2GOBJ (self));
  ClutterActor *g_actor = CLUTTER_ACTOR (RVAL2GOBJ (actor));
  Check_Type (properties, T_HASH);
  
  /* Ick.  Indeed, this truly is an annoying way to iterate through a
     hash. */
  gint c = 0;
  VALUE current_property = Qnil;
  gchar ** properties_vector = NULL;
  GValue * values_vector = NULL;

  VALUE props_lengthv = rb_funcall (properties, rb_intern("length"), 0);
  gint props_length = NUM2INT (props_lengthv);

  VALUE keys = rb_funcall (properties, rb_intern("keys"), 0);
  
  for (c = 0; c < props_length; c++)
    {
      VALUE key = rb_ary_entry (keys, c);
      current_property = rb_hash_aref (properties, key);
      GValue gv = {0, };
      
      if (current_property == Qnil) {
        rb_raise(rb_eArgError, "Properties must not be nil!");
        return Qnil;
      }
      
      rbgobj_initialize_gvalue(&gv, current_property);
      
      properties_vector[c] = StringValuePtr (key);
      values_vector[c] = gv;
      // printf("Hello: %s : %s \n", StringValuePtr (key), StringValuePtr (current_property));
    }
  
  clutter_box_packv (box, g_actor, props_length, (const gchar * const *)properties_vector, values_vector);

  g_free (properties_vector);
  g_free (values_vector);

  return Qnil;
}

void
rbclt_box_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BOX, "Box", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_box_initialize, 1);
  rb_define_method (klass, "set_layout_manager", rbclt_box_set_layout_manager, 1);
  rb_define_method (klass, "get_layout_manager", rbclt_box_get_layout_manager, 0);
  rb_define_method (klass, "set_color", rbclt_box_set_color, 1);
  rb_define_method (klass, "set_color", rbclt_box_get_color, 0);
  rb_define_method (klass, "pack", rbclt_box_pack, 2);
}
