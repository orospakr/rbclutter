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
#include <clutter/clutter.h>

#include "rbclutter.h"

static VALUE
rbclt_entry_initialize (int argc, VALUE *argv, VALUE self)
{
  VALUE font_name, text, color;
  gchar *font_name_s = NULL, *text_s = NULL;
  ClutterColor *color_s = NULL;
  ClutterActor *actor;

  rb_scan_args (argc, argv, "03", &font_name, &text, &color);

  if (!NIL_P (font_name))
    font_name_s = StringValuePtr (font_name);
  if (!NIL_P (text))
    text_s = StringValuePtr (text);
  if (!NIL_P (color))
    color_s = (ClutterColor *) RVAL2BOXED (color, CLUTTER_TYPE_COLOR);

  actor = clutter_entry_new ();

  if (font_name_s)
    clutter_entry_set_font_name (CLUTTER_ENTRY (actor), font_name_s);
  if (text_s)
    clutter_entry_set_text (CLUTTER_ENTRY (actor), text_s);
  if (color_s)
    clutter_entry_set_color (CLUTTER_ENTRY (actor), color_s);

  rbclt_initialize_unowned (self, actor);

  return Qnil;
}

static VALUE
rbclt_entry_layout (VALUE self)
{
  ClutterEntry *entry = CLUTTER_ENTRY (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_entry_get_layout (entry));
}

static VALUE
rbclt_entry_insert_unichar (VALUE self, VALUE wc)
{
  ClutterEntry *entry = CLUTTER_ENTRY (RVAL2GOBJ (self));
  clutter_entry_insert_unichar (entry, NUM2UINT (wc));
  return self;
}

static VALUE
rbclt_entry_delete_chars (VALUE self, VALUE len)
{
  ClutterEntry *entry = CLUTTER_ENTRY (RVAL2GOBJ (self));
  clutter_entry_delete_chars (entry, NUM2UINT (len));
  return self;
}

static VALUE
rbclt_entry_insert_text (int argc, VALUE *argv, VALUE self)
{
  ClutterEntry *entry = CLUTTER_ENTRY (RVAL2GOBJ (self));
  VALUE text, pos;
  gssize posi;

  rb_scan_args (argc, argv, "11", &text, &pos);

  if (pos == Qnil)
    posi = clutter_entry_get_cursor_position (entry);
  else
    posi = NUM2INT (pos);

  clutter_entry_insert_text (entry, StringValuePtr (text), posi);

  return self;
}

static VALUE
rbclt_entry_delete_text (VALUE self, VALUE start_pos, VALUE end_pos)
{
  ClutterEntry *entry = CLUTTER_ENTRY (RVAL2GOBJ (self));
  clutter_entry_delete_text (entry, NUM2INT (start_pos), NUM2INT (end_pos));
  return self;
}

static VALUE
rbclt_entry_set_invisible_char (VALUE self, VALUE wc)
{
  ClutterEntry *entry = CLUTTER_ENTRY (RVAL2GOBJ (self));
  clutter_entry_set_invisible_char (entry, NUM2UINT (wc));
  return self;
}

static VALUE
rbclt_entry_get_invisible_char (VALUE self)
{
  ClutterEntry *entry = CLUTTER_ENTRY (RVAL2GOBJ (self));
  return UINT2NUM (clutter_entry_get_invisible_char (entry));
}

static VALUE
rbclt_entry_set_position (int argc, VALUE *argv, VALUE self)
{
  ClutterEntry *entry = CLUTTER_ENTRY (RVAL2GOBJ (self));
  VALUE pos, y;

  rb_scan_args (argc, argv, "11", &pos, &y);

  /* If we were given two arguments then use the set_position method
     in Clutter::Actor instead */
  if (y != Qnil)
    rb_call_super (argc, argv);
  else
    clutter_entry_set_cursor_position (entry, NUM2INT (pos));

  return self;
}

void
rbclt_entry_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_ENTRY, "Entry", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_entry_initialize, -1);
  rb_define_method (klass, "layout", rbclt_entry_layout, 0);
  rb_define_method (klass, "insert_unichar", rbclt_entry_insert_unichar, 1);
  rb_define_method (klass, "delete_chars", rbclt_entry_delete_chars, 1);
  rb_define_method (klass, "insert_text", rbclt_entry_insert_text, -1);
  rb_define_method (klass, "delete_text", rbclt_entry_delete_text, 2);
  rb_define_method (klass, "set_invisible_char",
                    rbclt_entry_set_invisible_char, 1);
  rb_define_method (klass, "invisible_char", rbclt_entry_get_invisible_char, 0);
  rb_define_method (klass, "set_position", rbclt_entry_set_position, -1);
  rb_define_alias (klass, "set_cursor_position", "set_position");
  rb_define_alias (klass, "cursor_position", "position");
  rb_define_alias (klass, "cursor_position=", "position=");

  G_DEF_SETTERS (klass);
}
