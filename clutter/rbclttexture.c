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

#include <rbgobject.h>
#include <clutter/clutter.h>

#include "rbclutter.h"
#include "rbcltactor.h"

static VALUE rbclt_texture_error;

static VALUE
rbclt_texture_initialize (int argc, VALUE *argv, VALUE self)
{
  ClutterActor *actor;
  VALUE source;

  rb_scan_args (argc, argv, "01", &source);

  if (NIL_P (source))
    actor = clutter_texture_new ();
  else if (rb_obj_is_kind_of (source, rbclt_c_actor))
    {
      ClutterActor *source_actor = CLUTTER_ACTOR (RVAL2GOBJ (source));

      actor = clutter_texture_new_from_actor (source_actor);

      if (actor == NULL)
          rb_raise (rbclt_texture_error, "failed to create FBO");
    }
  else
    {
      GError *error = NULL;

      actor = clutter_texture_new_from_file (StringValuePtr (source), &error);

      if (error)
        RAISE_GERROR (error);
    }

  rbclt_initialize_unowned (self, actor);

  return Qnil;
}

static VALUE
rbclt_texture_set_from_file (VALUE self, VALUE filename)
{
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  GError *error = NULL;

  clutter_texture_set_from_file (texture, StringValuePtr (filename), &error);

  if (error)
    RAISE_GERROR (error);

  return self;
}

static VALUE
rbclt_texture_set_from_rgb_data (VALUE self, VALUE data, VALUE has_alpha,
                                 VALUE width_arg, VALUE height_arg,
                                 VALUE rowstride_arg,
                                 VALUE bpp_arg, VALUE flags_arg)
{
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  gint width = NUM2INT (width_arg), height = NUM2INT (height_arg);
  gint rowstride = NUM2INT (rowstride_arg), bpp = NUM2INT (bpp_arg);
  ClutterTextureFlags flags = RVAL2GFLAGS (flags_arg,
                                           CLUTTER_TYPE_TEXTURE_FLAGS);
  GError *error = NULL;

  /* Get the data as a string */
  data = StringValue (data);
  /* Make sure none of the arguments are negative */
  if (width < 0 || height < 0 || rowstride < 0 || bpp < 1)
    rb_raise (rb_eArgError, "bad value used for image data parameters");
  /* Make sure the data is large enough */
  if (RSTRING (data)->len < width * height * bpp
      || RSTRING (data)->len < height * rowstride)
    rb_raise (rb_eArgError, "string too small for image data");

  clutter_texture_set_from_rgb_data (texture,
                                     (guchar *) RSTRING (data)->ptr,
                                     RTEST (has_alpha),
                                     width, height,
                                     rowstride,
                                     bpp, flags,
                                     &error);

  if (error)
    RAISE_GERROR (error);

  return self;
}

static VALUE
rbclt_texture_set_area_from_rgb_data (VALUE self, VALUE data, VALUE has_alpha,
                                      VALUE x_arg, VALUE y_arg,
                                      VALUE width_arg, VALUE height_arg,
                                      VALUE rowstride_arg,
                                      VALUE bpp_arg, VALUE flags_arg)
{
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  gint x = NUM2INT (x_arg), y = NUM2INT (y_arg);
  gint width = NUM2INT (width_arg), height = NUM2INT (height_arg);
  gint rowstride = NUM2INT (rowstride_arg), bpp = NUM2INT (bpp_arg);
  ClutterTextureFlags flags = RVAL2GFLAGS (flags_arg,
                                           CLUTTER_TYPE_TEXTURE_FLAGS);
  GError *error = NULL;

  /* Get the data as a string */
  data = StringValue (data);
  /* Make sure none of the arguments are negative */
  if (width < 0 || height < 0 || rowstride < 0 || bpp < 1)
    rb_raise (rb_eArgError, "bad value used for image data parameters");
  /* Make sure the data is large enough */
  if (RSTRING (data)->len < width * height * bpp
      || RSTRING (data)->len < height * rowstride)
    rb_raise (rb_eArgError, "string too small for image data");

  clutter_texture_set_area_from_rgb_data (texture,
                                          (guchar *) RSTRING (data)->ptr,
                                          RTEST (has_alpha),
                                          x, y,
                                          width, height,
                                          rowstride,
                                          bpp, flags,
                                          &error);

  if (error)
    RAISE_GERROR (error);

  return self;
}

static VALUE
rbclt_texture_set_from_yuv_data (VALUE self, VALUE data,
                                 VALUE width_arg, VALUE height_arg,
                                 VALUE flags_arg)
{
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  gint width = NUM2INT (width_arg), height = NUM2INT (height_arg);
  ClutterTextureFlags flags = RVAL2GFLAGS (flags_arg,
                                           CLUTTER_TYPE_TEXTURE_FLAGS);
  GError *error = NULL;

  /* Get the data as a string */
  data = StringValue (data);
  /* Make sure none of the arguments are negative */
  if (width < 0 || height < 0)
    rb_raise (rb_eArgError, "bad value used for image data parameters");
  /* Make sure the data is large enough */
  if (RSTRING (data)->len < width * height * sizeof (gushort))
    rb_raise (rb_eArgError, "string too small for image data");

  clutter_texture_set_from_yuv_data (texture,
                                     (guchar *) RSTRING (data)->ptr,
                                     width, height, flags,
                                     &error);

  if (error)
    RAISE_GERROR (error);

  return self;
}

static VALUE
rbclt_texture_base_size (VALUE self)
{
  gint width, height;
  ClutterTexture *texture = CLUTTER_TEXTURE (RVAL2GOBJ (self));
  clutter_texture_get_base_size (texture, &width, &height);
  return rb_ary_new3 (2, INT2NUM (width), INT2NUM (height));
}

void
rbclt_texture_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_TEXTURE, "Texture", rbclt_c_clutter);

  rbclt_texture_error
    = G_DEF_ERROR (CLUTTER_TEXTURE_ERROR, "TextureError", rbclt_c_clutter,
                   rbclt_c_clutter_error,
                   CLUTTER_TYPE_TEXTURE_ERROR);

  G_DEF_CLASS (CLUTTER_TYPE_TEXTURE_FLAGS, "Flags", klass);
  G_DEF_CONSTANTS (klass, CLUTTER_TYPE_TEXTURE_FLAGS, "CLUTTER_TEXTURE_");

  G_DEF_CLASS (CLUTTER_TYPE_TEXTURE_QUALITY, "Quality", klass);
  G_DEF_CONSTANTS (klass, CLUTTER_TYPE_TEXTURE_QUALITY,
                   "CLUTTER_TEXTURE_QUALITY_");

  rb_define_method (klass, "initialize", rbclt_texture_initialize, -1);
  rb_define_method (klass, "set_from_file", rbclt_texture_set_from_file, 1);
  rb_define_method (klass, "set_from_rgb_data",
                    rbclt_texture_set_from_rgb_data, 7);
  rb_define_method (klass, "set_from_yuv_data",
                    rbclt_texture_set_from_yuv_data, 4);
  rb_define_method (klass, "set_area_from_rgb_data",
                    rbclt_texture_set_area_from_rgb_data, 9);
  rb_define_method (klass, "base_size", rbclt_texture_base_size, 0);

  G_DEF_SETTERS (klass);
}
