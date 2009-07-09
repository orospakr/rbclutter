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
rbclt_text_initialize (int argc, VALUE *argv, VALUE self)
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

  actor = clutter_text_new ();

  if (font_name_s)
    clutter_text_set_font_name (CLUTTER_TEXT (actor), font_name_s);
  if (text_s)
    clutter_text_set_text (CLUTTER_TEXT (actor), text_s);
  if (color_s)
    clutter_text_set_color (CLUTTER_TEXT (actor), color_s);

  rbclt_initialize_unowned (self, actor);

  return Qnil;
}

static VALUE
rbclt_text_layout (VALUE self)
{
  ClutterText *text = CLUTTER_TEXT (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_text_get_layout (text));
}

void
rbclt_text_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_TEXT, "Text", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_text_initialize, -1);
  rb_define_method (klass, "layout", rbclt_text_layout, 0);
}
