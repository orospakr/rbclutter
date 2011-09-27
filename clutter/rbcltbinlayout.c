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

/* http://docs.clutter-project.org/docs/clutter/stable/ClutterBinLayout.html */

static VALUE
rbclt_bin_layout_initialize (VALUE self, VALUE x_align, VALUE y_align)
{
  rbclt_initialize_unowned (self, clutter_bin_layout_new(RVAL2GENUM (x_align, CLUTTER_TYPE_BIN_ALIGNMENT), RVAL2GENUM (y_align, CLUTTER_TYPE_BIN_ALIGNMENT)));

  return Qnil;
}

static VALUE
rbclt_bin_layout_set_alignment (VALUE self, VALUE child, VALUE x_align, VALUE y_align)
{
  ClutterBinLayout *layout = CLUTTER_BIN_LAYOUT (RVAL2GOBJ (self));
  ClutterActor *g_child = CLUTTER_ACTOR (RVAL2GOBJ (self));

  clutter_bin_layout_set_alignment (layout, g_child, RVAL2GENUM (x_align, CLUTTER_TYPE_BIN_ALIGNMENT), RVAL2GENUM (y_align, CLUTTER_TYPE_BIN_ALIGNMENT));
  return Qnil;
}

static VALUE
rbclt_bin_layout_get_alignment (VALUE self, VALUE child)
{
  ClutterBinLayout *layout = CLUTTER_BIN_LAYOUT (RVAL2GOBJ (self));
  ClutterActor *g_child = CLUTTER_ACTOR (RVAL2GOBJ (self));
  VALUE answer = rb_hash_new ();
  ClutterBinAlignment x_align, y_align;

  clutter_bin_layout_get_alignment (layout, g_child, &x_align, &y_align);
  rb_hash_aset (answer, rb_intern("x"), GENUM2RVAL (x_align, CLUTTER_TYPE_BIN_ALIGNMENT));
  rb_hash_aset (answer, rb_intern("y"), GENUM2RVAL (y_align, CLUTTER_TYPE_BIN_ALIGNMENT));
  return answer;
}

static VALUE
rbclt_bin_layout_add (VALUE self, VALUE child, VALUE x_align, VALUE y_align)
{
  ClutterBinLayout *layout = CLUTTER_BIN_LAYOUT (RVAL2GOBJ (self));
  ClutterActor *g_child = CLUTTER_ACTOR (RVAL2GOBJ (self));

  clutter_bin_layout_add (layout, g_child, RVAL2GENUM (x_align, CLUTTER_TYPE_BIN_ALIGNMENT), RVAL2GENUM (y_align, CLUTTER_TYPE_BIN_ALIGNMENT));

  return Qnil;
}

void
rbclt_bin_layout_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BIN_LAYOUT, "BinLayout", rbclt_c_clutter);

  rb_define_method (klass, "initalize", rbclt_bin_layout_initialize, 2);
  rb_define_method (klass, "set_alignment", rbclt_bin_layout_set_alignment, 3);
  rb_define_method (klass, "get_alignment", rbclt_bin_layout_get_alignment, 1);
  rb_define_method (klass, "add", rbclt_bin_layout_add, 3);
}
