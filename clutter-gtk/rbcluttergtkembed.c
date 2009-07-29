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

#include <ruby.h>
#include <rbgobject.h>
#include <rbgtk.h>
#include <glib-object.h>
#include <clutter-gtk/clutter-gtk.h>
#include <rbclutter.h>

static VALUE
rbcltg_embed_initialize (VALUE self)
{
  GtkWidget *widget = gtk_clutter_embed_new ();

  rbclt_initialize_unowned (self, widget);

  return Qnil;
}

static VALUE
rbcltg_embed_get_stage (VALUE self)
{
  GtkClutterEmbed *gtk = GTK_CLUTTER_EMBED (RVAL2GOBJ (self));
  return GOBJ2RVAL (gtk_clutter_embed_get_stage (gtk));
}

void
rbcltg_embed_init (void)
{
  VALUE klass;

  klass = G_DEF_CLASS (GTK_CLUTTER_TYPE_EMBED, "GtkEmbed", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbcltg_embed_initialize, 0);
  rb_define_method (klass, "stage", rbcltg_embed_get_stage, 0);
}
