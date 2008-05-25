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

#include <ruby.h>
#include <rbgobject.h>
#include <rbgtk.h>
#include <glib-object.h>
#include <clutter-gtk/clutter-gtk.h>

static VALUE rbcltg_c_clutter = Qnil;

static void
rbcltg_initialize_unowned (VALUE obj, gpointer gobj)
{
  /* If the object is a subclass of GInitiallyUnowned then grab the
     floating reference */
  if (G_IS_INITIALLY_UNOWNED (gobj))
    g_object_ref_sink (gobj);
  G_INITIALIZE (obj, gobj);
}

static VALUE
rbcltg_gtk_initialize (VALUE self)
{
  GtkWidget *widget = gtk_clutter_new ();

  rbcltg_initialize_unowned (self, widget);

  return Qnil;
}

static VALUE
rbcltg_gtk_get_stage (VALUE self)
{
  GtkClutter *gtk = GTK_CLUTTER (RVAL2GOBJ (self));
  return GOBJ2RVAL (gtk_clutter_get_stage (gtk));
}

void
Init_clutter_gtk ()
{
  VALUE klass;

  rb_require ("gtk2");
  rb_require ("clutter");

  rbcltg_c_clutter = rb_define_module ("Clutter");

  klass = G_DEF_CLASS (GTK_TYPE_CLUTTER, "Gtk", rbcltg_c_clutter);
  rb_define_method (klass, "initialize", rbcltg_gtk_initialize, 0);
  rb_define_method (klass, "stage", rbcltg_gtk_get_stage, 0);
}
