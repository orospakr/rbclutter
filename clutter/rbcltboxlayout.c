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

/* http://docs.clutter-project.org/docs/clutter/stable/ClutterBoxLayout.html */

static VALUE
rbclt_box_layout_initialize (VALUE self)
{
  rbclt_initialize_unowned (self, clutter_box_layout_new ());
  return Qnil;
}

static VALUE
rbclt_box_layout_pack (VALUE self, VALUE actor, VALUE expand, VALUE x_fill, VALUE y_fill, VALUE x_align, VALUE y_align)
{
  ClutterBoxLayout *layout = CLUTTER_BOX_LAYOUT (RVAL2GOBJ (self));
  ClutterActor *g_actor = CLUTTER_ACTOR (RVAL2GOBJ (self));

  clutter_box_layout_pack (layout, g_actor, RVAL2CBOOL (expand), RVAL2CBOOL (x_fill), RVAL2CBOOL (y_fill), RVAL2GENUM (x_align, CLUTTER_TYPE_BOX_ALIGNMENT), RVAL2GENUM (y_align, CLUTTER_TYPE_BOX_ALIGNMENT));
  return Qnil;
}

static VALUE
rbclt_box_layout_set_alignment (VALUE self, VALUE actor, VALUE x_align, VALUE y_align)
{
  ClutterBoxLayout *layout = CLUTTER_BOX_LAYOUT (RVAL2GOBJ (self));
  ClutterActor *g_actor = CLUTTER_ACTOR (RVAL2GOBJ (self));

  clutter_box_layout_set_alignment (layout, g_actor, RVAL2GENUM (x_align, CLUTTER_TYPE_BOX_ALIGNMENT), RVAL2GENUM (y_align, CLUTTER_TYPE_BOX_ALIGNMENT));

  return Qnil;
}

static VALUE
rbclt_box_layout_get_alignment (VALUE self, VALUE actor)
{
  ClutterBoxLayout *layout = CLUTTER_BOX_LAYOUT (RVAL2GOBJ (self));
  ClutterActor *g_actor = CLUTTER_ACTOR (RVAL2GOBJ (self));

  ClutterBoxAlignment x_align, y_align;
  VALUE answer = rb_hash_new ();

  clutter_box_layout_get_alignment (layout, g_actor, &x_align, &y_align);
  rb_hash_aset (answer, rb_intern ("x"), GENUM2RVAL (x_align, CLUTTER_TYPE_BOX_ALIGNMENT));
  rb_hash_aset (answer, rb_intern ("y"), GENUM2RVAL (y_align, CLUTTER_TYPE_BOX_ALIGNMENT));
  return answer;
}

static VALUE
rbclt_box_layout_set_expand (VALUE self, VALUE actor, VALUE expand)
{
  ClutterBoxLayout *layout = CLUTTER_BOX_LAYOUT (RVAL2GOBJ (self));
  ClutterActor *g_actor = CLUTTER_ACTOR (RVAL2GOBJ (self));

  clutter_box_layout_set_expand (layout, g_actor, RVAL2CBOOL (expand));

  return Qnil;
}

static VALUE
rbclt_box_layout_get_expand (VALUE self, VALUE actor)
{
  ClutterBoxLayout *layout = CLUTTER_BOX_LAYOUT (RVAL2GOBJ (self));
  ClutterActor *g_actor = CLUTTER_ACTOR (RVAL2GOBJ (self));

  return CBOOL2RVAL (clutter_box_layout_get_expand (layout, g_actor));
}

static VALUE
rbclt_box_layout_set_fill (VALUE self, VALUE actor, VALUE x_fill, VALUE y_fill)
{
  ClutterBoxLayout *layout = CLUTTER_BOX_LAYOUT (RVAL2GOBJ (self));
  ClutterActor *g_actor = CLUTTER_ACTOR (RVAL2GOBJ (self));

  clutter_box_layout_set_fill (layout, g_actor, RVAL2CBOOL (x_fill), RVAL2CBOOL (y_fill));

  return Qnil;
}

static VALUE
rbclt_box_layout_get_fill (VALUE self, VALUE actor)
{
  ClutterBoxLayout *layout = CLUTTER_BOX_LAYOUT (RVAL2GOBJ (self));
  ClutterActor *g_actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  gboolean x_fill, y_fill;
  VALUE answer = rb_hash_new ();
  
  clutter_box_layout_get_fill (layout, g_actor, &x_fill, &y_fill);

  rb_hash_aset (answer, rb_intern("x"), CBOOL2RVAL (x_fill));
  rb_hash_aset (answer, rb_intern("y"), CBOOL2RVAL (y_fill));
  return answer;
}

void
rbclt_box_layout_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BOX_LAYOUT, "BoxLayout", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_box_layout_initialize, 0);
  rb_define_method (klass, "pack", rbclt_box_layout_pack, 6);
  rb_define_method (klass, "set_alignment", rbclt_box_layout_set_alignment, 3);
  rb_define_method (klass, "get_alignment", rbclt_box_layout_get_alignment, 1);
  rb_define_method (klass, "set_expand", rbclt_box_layout_set_expand, 2);
  rb_define_method (klass, "get_expand", rbclt_box_layout_get_expand, 1);
  rb_define_method (klass, "set_fill", rbclt_box_layout_set_fill, 3);
  rb_define_method (klass, "get_fill", rbclt_box_layout_get_fill, 1);
}
