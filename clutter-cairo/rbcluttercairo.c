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
#include <rb_cairo.h>
#include <cairo.h>
#include <glib-object.h>
#include <clutter-cairo/clutter-cairo.h>

static VALUE rbcltc_c_clutter = Qnil;

static VALUE rbcltc_cairo_context_finish (VALUE self);

static void
rbcltc_initialize_unowned (VALUE obj, gpointer gobj)
{
  /* If the object is a subclass of GInitiallyUnowned then grab the
     floating reference */
  if (G_IS_INITIALLY_UNOWNED (gobj))
    g_object_ref_sink (gobj);
  G_INITIALIZE (obj, gobj);
}

static VALUE
rbcltc_cairo_initialize (VALUE self, VALUE width, VALUE height)
{
  ClutterActor *actor = clutter_cairo_new (NUM2UINT (width), NUM2UINT (height));

  rbcltc_initialize_unowned (self, actor);

  return Qnil;
}

static VALUE
rbcltc_cairo_set_source_color (VALUE self, VALUE cairo, VALUE color)
{
  clutter_cairo_set_source_color (RVAL2CRCONTEXT (cairo),
				  (ClutterColor *) RVAL2BOXED (color, CLUTTER_TYPE_COLOR));

  return cairo;
}

static VALUE
rbcltc_cairo_create (VALUE self)
{
  ClutterCairo *cairo = CLUTTER_CAIRO (RVAL2GOBJ (self));
  cairo_t *cr = clutter_cairo_create (cairo);
  VALUE ret = CRCONTEXT2RVAL (cr);
  /* The context returned by clutter_cairo_create has a reference
     owned by us, but CRCONTEXT2RVAL creates another reference held by
     Ruby, so we should drop the original reference to let Ruby handle
     it */
  cairo_destroy (cr);

  /* If a block is given then evaluate that block with the context and
     call finish when it is over */
  if (rb_block_given_p ())
    return rb_ensure (rb_yield, ret, rbcltc_cairo_context_finish, ret);
  else
    return ret;
}

static VALUE
rbcltc_cairo_context_finish (VALUE self)
{
  cairo_surface_t *surface;
  cairo_t *new_cr, *old_cr;

  /* Make a new stub cairo context so the Ruby object will still be
     valid */
  surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 1, 1);
  new_cr = cairo_create (surface);
  cairo_surface_destroy (surface);

  /* Replace the old cairo context with the stub */
  Data_Get_Struct (self, cairo_t, old_cr);
  DATA_PTR (self) = new_cr;

  /* Destroy the old context so that Clutter will update the texture */
  cairo_destroy (old_cr);
  
  return self;
}

void
Init_clutter_cairo ()
{
  VALUE klass;

  rb_require ("cairo");
  rb_require ("clutter");

  rbcltc_c_clutter = rb_define_module ("Clutter");

  klass = G_DEF_CLASS (CLUTTER_TYPE_CAIRO, "Cairo", rbcltc_c_clutter);
  rb_define_method (klass, "initialize", rbcltc_cairo_initialize, 2);
  rb_define_method (klass, "create", rbcltc_cairo_create, 0);
  rb_define_singleton_method (klass, "set_source_color", rbcltc_cairo_set_source_color, 2);

  /* The Ruby bindings of Cairo have no 'destroy' method. Instead this
     is called when the object gets reaped by the garbage
     collecter. However in clutter-cairo, the cairo context needs to
     be explicitly destroyed before clutter will update the
     texture. This method works around the problem by replacing the
     cairo context in the ruby object with a new stub context so that
     the original context can be destroyed without ruining the ruby
     object */
  rb_define_method (rb_const_get (rb_const_get (rb_cObject, rb_intern ("Cairo")),
				  rb_intern ("Context")),
		    "finish", rbcltc_cairo_context_finish, 0);
}
