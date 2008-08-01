/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2008  Neil Roberts
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

#include <ruby.h>
#include <rbgobject.h>
#include <rbgtk.h>
#include <glib-object.h>
#include <clutter-gtk/gtk-clutter-util.h>
#include <clutter-gtk/gtk-clutter-embed.h>
#include <rbclutter.h>

extern void rbcltg_embed_init (void);

static VALUE
rbcltg_init (int argc, VALUE *argv, VALUE self)
{
  return rbclt_call_init_func (argc, argv, gtk_clutter_init);
}

static VALUE
rbcltg_get_fg_color (VALUE self, VALUE widget, VALUE state_arg)
{
  GtkStateType state = RVAL2GENUM (state_arg, GTK_TYPE_STATE_TYPE);
  ClutterColor color;

  gtk_clutter_get_fg_color (RVAL2GOBJ (widget), state, &color);

  return BOXED2RVAL (&color, CLUTTER_TYPE_COLOR);
}

static VALUE
rbcltg_get_bg_color (VALUE self, VALUE widget, VALUE state_arg)
{
  GtkStateType state = RVAL2GENUM (state_arg, GTK_TYPE_STATE_TYPE);
  ClutterColor color;

  gtk_clutter_get_bg_color (RVAL2GOBJ (widget), state, &color);

  return BOXED2RVAL (&color, CLUTTER_TYPE_COLOR);
}

static VALUE
rbcltg_get_text_color (VALUE self, VALUE widget, VALUE state_arg)
{
  GtkStateType state = RVAL2GENUM (state_arg, GTK_TYPE_STATE_TYPE);
  ClutterColor color;

  gtk_clutter_get_text_color (RVAL2GOBJ (widget), state, &color);

  return BOXED2RVAL (&color, CLUTTER_TYPE_COLOR);
}

static VALUE
rbcltg_texture_new_from_pixbuf (VALUE self, VALUE pixbuf_arg)
{
  GdkPixbuf *pixbuf = (GdkPixbuf *) RVAL2GOBJ (pixbuf_arg);
  ClutterActor *actor = gtk_clutter_texture_new_from_pixbuf (pixbuf);

  g_object_ref_sink (actor);

  return GOBJ2RVALU (actor);
}

static VALUE
rbcltg_texture_new_from_stock (VALUE self, VALUE widget_arg,
			       VALUE stock_id, VALUE size_arg)
{
  GtkWidget *widget = (GtkWidget *) RVAL2GOBJ (widget_arg);
  GtkIconSize size = RVAL2GENUM (size_arg, GTK_TYPE_ICON_SIZE);
  ClutterActor *actor
    = gtk_clutter_texture_new_from_stock (widget,
					  StringValuePtr (stock_id),
					  size);

  g_object_ref_sink (actor);

  return GOBJ2RVALU (actor);
}

static VALUE
rbcltg_texture_new_from_icon_name (VALUE self, VALUE widget_arg,
				   VALUE icon_name, VALUE size_arg)
{
  GtkWidget *widget = (GtkWidget *) RVAL2GOBJ (widget_arg);
  GtkIconSize size = RVAL2GENUM (size_arg, GTK_TYPE_ICON_SIZE);
  ClutterActor *actor
    = gtk_clutter_texture_new_from_icon_name (widget,
					      StringValuePtr (icon_name),
					      size);

  g_object_ref_sink (actor);

  return GOBJ2RVALU (actor);
}

static VALUE
rbcltg_texture_set_from_pixbuf (VALUE self, VALUE texture_arg, VALUE pixbuf_arg)
{
  ClutterTexture *texture = (ClutterTexture *) RVAL2GOBJ (texture_arg);
  GdkPixbuf *pixbuf = (GdkPixbuf *) RVAL2GOBJ (pixbuf_arg);

  gtk_clutter_texture_set_from_pixbuf (texture, pixbuf);

  return Qnil;
}

static VALUE
rbcltg_texture_set_from_stock (VALUE self, VALUE texture_arg, VALUE widget_arg,
			       VALUE stock_id, VALUE size_arg)
{
  ClutterTexture *texture = (ClutterTexture *) RVAL2GOBJ (texture_arg);
  GtkWidget *widget = (GtkWidget *) RVAL2GOBJ (widget_arg);
  GtkIconSize size = RVAL2GENUM (size_arg, GTK_TYPE_ICON_SIZE);

  gtk_clutter_texture_set_from_stock (texture,
				      widget,
				      StringValuePtr (stock_id),
				      size);

  return Qnil;
}

static VALUE
rbcltg_texture_set_from_icon_name (VALUE self, VALUE texture_arg,
				   VALUE widget_arg,
				   VALUE icon_name, VALUE size_arg)
{
  ClutterTexture *texture = (ClutterTexture *) RVAL2GOBJ (texture_arg);
  GtkWidget *widget = (GtkWidget *) RVAL2GOBJ (widget_arg);
  GtkIconSize size = RVAL2GENUM (size_arg, GTK_TYPE_ICON_SIZE);

  gtk_clutter_texture_set_from_icon_name (texture,
					  widget,
					  StringValuePtr (icon_name),
					  size);

  return Qnil;
}

void
Init_clutter_gtk (void)
{
  VALUE klass;

  rb_require ("gtk2");
  rb_require ("clutter");

  klass = rb_define_module_under (rbclt_c_clutter, "Gtk");

  rb_define_singleton_method (klass, "init", rbcltg_init, -1);
  rb_define_singleton_method (klass, "get_fg_color", rbcltg_get_fg_color, 2);
  rb_define_singleton_method (klass, "get_bg_color", rbcltg_get_bg_color, 2);
  rb_define_singleton_method (klass, "get_text_color",
			      rbcltg_get_text_color, 2);
  rb_define_singleton_method (klass, "texture_new_from_pixbuf",
			      rbcltg_texture_new_from_pixbuf, 1);
  rb_define_singleton_method (klass, "texture_new_from_stock",
			      rbcltg_texture_new_from_stock, 3);
  rb_define_singleton_method (klass, "texture_new_from_icon_name",
			      rbcltg_texture_new_from_icon_name, 3);
  rb_define_singleton_method (klass, "texture_set_from_pixbuf",
			      rbcltg_texture_set_from_pixbuf, 2);
  rb_define_singleton_method (klass, "texture_set_from_stock",
			      rbcltg_texture_set_from_stock, 4);
  rb_define_singleton_method (klass, "texture_set_from_icon_name",
			      rbcltg_texture_set_from_icon_name, 4);

  rbcltg_embed_init ();
}
