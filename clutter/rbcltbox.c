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
#include <clutter/clutter-box.h>
#include <clutter/clutter-hbox.h>
#include <clutter/clutter-vbox.h>
#include <clutter/clutter-types.h>
#include <clutter/clutter-enum-types.h>

#include "rbclutter.h"

static VALUE
rbclt_box_set_default_padding (int argc, VALUE *argv, VALUE self)
{
  ClutterBox *box = CLUTTER_BOX (RVAL2GOBJ (self));

  if (argc == 1)
    {
      ClutterPadding *padding = (ClutterPadding *) RVAL2BOXED (argv[0], CLUTTER_TYPE_PADDING);
      clutter_box_set_default_padding (box,
				       CLUTTER_UNITS_TO_INT (padding->top),
				       CLUTTER_UNITS_TO_INT (padding->right),
				       CLUTTER_UNITS_TO_INT (padding->bottom),
				       CLUTTER_UNITS_TO_INT (padding->left));
    }
  else
    {
      VALUE top, right, bottom, left;
      rb_scan_args (argc, argv, "40", &top, &right, &bottom, &left);
      clutter_box_set_default_padding (box, NUM2INT (top), NUM2INT (right), NUM2INT (bottom), NUM2INT (left));
    }

  return self;
}

static VALUE
rbclt_box_default_padding_equals (VALUE self, VALUE padding_arg)
{
  ClutterBox *box = CLUTTER_BOX (RVAL2GOBJ (self));
  ClutterPadding *padding = (ClutterPadding *) RVAL2BOXED (padding_arg, CLUTTER_TYPE_PADDING);
  clutter_box_set_default_padding (box,
				   CLUTTER_UNITS_TO_INT (padding->top),
				   CLUTTER_UNITS_TO_INT (padding->right),
				   CLUTTER_UNITS_TO_INT (padding->bottom),
				   CLUTTER_UNITS_TO_INT (padding->left));
  return padding_arg;
}

static VALUE
rbclt_box_get_default_padding (VALUE self)
{
  ClutterBox *box = CLUTTER_BOX (RVAL2GOBJ (self));
  gint top, right, bottom, left;
  ClutterPadding padding;

  clutter_box_get_default_padding (box, &top, &right, &bottom, &left);
  padding.top = CLUTTER_UNITS_FROM_INT (top);
  padding.right = CLUTTER_UNITS_FROM_INT (right);
  padding.bottom = CLUTTER_UNITS_FROM_INT (bottom);
  padding.left = CLUTTER_UNITS_FROM_INT (left);

  return BOXED2RVAL (&padding, CLUTTER_TYPE_PADDING);
}

static VALUE
rbclt_box_pack (int argc, VALUE *argv, VALUE self)
{
  ClutterBox *box = CLUTTER_BOX (RVAL2GOBJ (self));
  VALUE actor, pack_type, padding_arg;
  ClutterPadding padding;

  rb_scan_args (argc, argv, "12", &actor, &pack_type, &padding_arg);

  if (padding_arg == Qnil)
    {
      gint top, right, bottom, left;
      clutter_box_get_default_padding (box, &top, &right, &bottom, &left);
      padding.top = CLUTTER_UNITS_FROM_INT (top);
      padding.right = CLUTTER_UNITS_FROM_INT (right);
      padding.bottom = CLUTTER_UNITS_FROM_INT (bottom);
      padding.left = CLUTTER_UNITS_FROM_INT (left);
    }
  else
    padding = *(ClutterPadding *) RVAL2BOXED (padding_arg, CLUTTER_TYPE_PADDING);

  clutter_box_pack (box, RVAL2GOBJ (actor),
		    pack_type == Qnil ? CLUTTER_PACK_START
		    : RVAL2GENUM (pack_type, CLUTTER_TYPE_PACK_TYPE),
		    &padding);

  return self;
}

static VALUE
rbclt_box_remove_all (VALUE self)
{
  ClutterBox *box = CLUTTER_BOX (RVAL2GOBJ (self));
  clutter_box_remove_all (box);
  return self;
}

static VALUE
rbclt_box_child_to_rvalue (ClutterBoxChild *child)
{
  return rb_ary_new3 (4, GOBJ2RVAL (child->actor),
		      BOXED2RVAL (&child->child_coords, CLUTTER_TYPE_ACTOR_BOX),
		      GENUM2RVAL (child->pack_type, CLUTTER_TYPE_PACK_TYPE),
		      BOXED2RVAL (&child->padding, CLUTTER_TYPE_PADDING));
}

static VALUE
rbclt_box_query_child (VALUE self, VALUE actor)
{
  ClutterBox *box = CLUTTER_BOX (RVAL2GOBJ (self));
  ClutterBoxChild child;

  if (clutter_box_query_child (box, RVAL2GOBJ (actor), &child))
    return rbclt_box_child_to_rvalue (&child);
  else
    return Qnil;
}

static VALUE
rbclt_box_query_nth_child (VALUE self, VALUE index)
{
  ClutterBox *box = CLUTTER_BOX (RVAL2GOBJ (self));
  ClutterBoxChild child;

  if (clutter_box_query_nth_child (box, NUM2INT (index), &child))
    return rbclt_box_child_to_rvalue (&child);
  else
    return Qnil;
}

static VALUE
rbclt_hbox_initialize (VALUE self)
{
  ClutterActor *actor;
  actor = clutter_hbox_new ();
  rbclt_initialize_unowned (self, actor);
  return Qnil;
}

static VALUE
rbclt_vbox_initialize (VALUE self)
{
  ClutterActor *actor;
  actor = clutter_vbox_new ();
  rbclt_initialize_unowned (self, actor);
  return Qnil;
}

void
rbclt_box_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BOX, "Box", rbclt_c_clutter);

  G_DEF_CLASS (CLUTTER_TYPE_PACK_TYPE, "PackType", rbclt_c_clutter);
  G_DEF_CONSTANTS (rbclt_c_clutter, CLUTTER_TYPE_PACK_TYPE, "CLUTTER_");

  rb_define_method (klass, "set_default_padding", rbclt_box_set_default_padding, -1);
  rb_define_method (klass, "default_padding=", rbclt_box_default_padding_equals, 1);
  rb_define_method (klass, "default_padding", rbclt_box_get_default_padding, 0);
  rb_define_method (klass, "pack", rbclt_box_pack, -1);
  rb_define_method (klass, "remove_all", rbclt_box_remove_all, 0);
  rb_define_method (klass, "query_child", rbclt_box_query_child, 1);
  rb_define_method (klass, "query_nth_child", rbclt_box_query_nth_child, 1);

  klass = G_DEF_CLASS (CLUTTER_TYPE_HBOX, "HBox", rbclt_c_clutter);
  rb_define_method (klass, "initialize", rbclt_hbox_initialize, 0);

  klass = G_DEF_CLASS (CLUTTER_TYPE_VBOX, "VBox", rbclt_c_clutter);
  rb_define_method (klass, "initialize", rbclt_vbox_initialize, 0);
}
