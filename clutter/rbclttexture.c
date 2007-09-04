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
#include <clutter/clutter-texture.h>
#include <clutter/clutter-enum-types.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "rbclutter.h"

static VALUE
rbclt_texture_initialize (int argc, VALUE *argv, VALUE self)
{
  ClutterActor *actor;
  VALUE pixbuf;

  rb_scan_args (argc, argv, "01", &pixbuf);

  if (NIL_P (pixbuf))
    actor = clutter_texture_new ();
  else
    actor = clutter_texture_new_from_pixbuf (GDK_PIXBUF (RVAL2GOBJ (pixbuf)));

  rbclt_initialize_unowned (self, actor);

  return Qnil;
}

static VALUE
rbclt_texture_set_from_rgb_data (VALUE self, VALUE data, VALUE has_alpha,
				 VALUE width_arg, VALUE height_arg, VALUE rowstride_arg,
				 VALUE bpp_arg, VALUE flags)
{
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  gint width = NUM2INT (width_arg), height = NUM2INT (height_arg);
  gint rowstride = NUM2INT (rowstride_arg), bpp = NUM2INT (bpp_arg);
  GError *error = NULL;
  gboolean ret;

  /* Get the data as a string */
  data = StringValue (data);
  /* Make sure none of the arguments are negative */
  if (width < 0 || height < 0 || rowstride < 0 || bpp < 1)
    rb_raise (rb_eArgError, "bad value used for image data parameters");
  /* Make sure the data is large enough */
  if (RSTRING (data)->len < width * height * bpp
      || RSTRING (data)->len < height * rowstride)
    rb_raise (rb_eArgError, "string too small for image data");

  ret = clutter_texture_set_from_rgb_data (texture, (guchar *) RSTRING (data)->ptr,
					   RTEST (has_alpha), width, height, rowstride,
					   bpp, RVAL2GFLAGS (flags, CLUTTER_TYPE_TEXTURE_FLAGS),
					   &error);

  if (error)
    RAISE_GERROR (error);

  return ret ? Qtrue : Qfalse;
}

static VALUE
rbclt_texture_set_from_yuv_data (VALUE self, VALUE data,
				 VALUE width_arg, VALUE height_arg,
				 VALUE flags)
{
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  gint width = NUM2INT (width_arg), height = NUM2INT (height_arg);
  GError *error = NULL;
  gboolean ret;

  /* Get the data as a string */
  data = StringValue (data);
  /* Make sure none of the arguments are negative */
  if (width < 0 || height < 0)
    rb_raise (rb_eArgError, "bad value used for image data parameters");
  /* Make sure the data is large enough */
  if (RSTRING (data)->len < width * height * sizeof (gushort))
    rb_raise (rb_eArgError, "string too small for image data");

  ret = clutter_texture_set_from_yuv_data (texture, (guchar *) RSTRING (data)->ptr,
					   width, height, 
					   RVAL2GFLAGS (flags, CLUTTER_TYPE_TEXTURE_FLAGS),
					   &error);

  if (error)
    RAISE_GERROR (error);

  return ret ? Qtrue : Qfalse;
}

static VALUE
rbclt_texture_base_size (VALUE self)
{
  gint width, height;
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  clutter_texture_get_base_size (texture, &width, &height);
  return rb_ary_new3 (2, INT2NUM (width), INT2NUM (height));
}

static VALUE
rbclt_texture_bind_tile (VALUE self, VALUE index)
{
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  clutter_texture_bind_tile (texture, NUM2INT (index));
  return self;
}

static VALUE
rbclt_texture_n_tiles (VALUE self)
{
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  gint n_x_tiles, n_y_tiles;
  clutter_texture_get_n_tiles (texture, &n_x_tiles, &n_y_tiles);
  return rb_ary_new3 (2, INT2NUM (n_x_tiles), INT2NUM (n_y_tiles));
}

static VALUE
rbclt_texture_x_tile_detail (VALUE self, VALUE x_index)
{
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  gint pos, size, waste;
  clutter_texture_get_x_tile_detail (texture, NUM2INT (x_index), &pos, &size, &waste);
  return rb_ary_new3 (3, INT2NUM (pos), INT2NUM (size), INT2NUM (waste));
}

static VALUE
rbclt_texture_y_tile_detail (VALUE self, VALUE y_index)
{
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  gint pos, size, waste;
  clutter_texture_get_x_tile_detail (texture, NUM2INT (y_index), &pos, &size, &waste);
  return rb_ary_new3 (3, INT2NUM (pos), INT2NUM (size), INT2NUM (waste));
}

static VALUE
rbclt_texture_has_generated_tiles (VALUE self)
{
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  return clutter_texture_has_generated_tiles (texture) ? Qtrue : Qfalse;
}

void
rbclt_texture_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_TEXTURE, "Texture", rbclt_c_clutter);

  G_DEF_ERROR (CLUTTER_TEXTURE_ERROR, "Error", klass, rb_eRuntimeError,
	       CLUTTER_TYPE_TEXTURE_ERROR);

  G_DEF_CLASS (CLUTTER_TYPE_TEXTURE_FLAGS, "Flags", klass);
  G_DEF_CONSTANTS (klass, CLUTTER_TYPE_TEXTURE_FLAGS, "CLUTTER_TEXTURE_");

  rb_define_method (klass, "initialize", rbclt_texture_initialize, -1);
  rb_define_method (klass, "set_from_rgb_data", rbclt_texture_set_from_rgb_data, 7);
  rb_define_method (klass, "set_from_yuv_data", rbclt_texture_set_from_yuv_data, 4);
  rb_define_method (klass, "base_size", rbclt_texture_base_size, 0);
  rb_define_method (klass, "bind_tile", rbclt_texture_bind_tile, 1);
  rb_define_method (klass, "n_tiles", rbclt_texture_n_tiles, 0);
  rb_define_method (klass, "x_tile_detail", rbclt_texture_x_tile_detail, 1);
  rb_define_method (klass, "y_tile_detail", rbclt_texture_y_tile_detail, 1);
  rb_define_method (klass, "has_generated_tiles?", rbclt_texture_has_generated_tiles, 0);
  
  G_DEF_SETTERS (klass);
}
