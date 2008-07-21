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

#include <rbgobject.h>
#include <cogl/cogl.h>
#include <clutter/clutter-texture.h>

#include "rbclutter.h"
#include "rbcogltexture.h"

static VALUE rb_c_cogl_texture_error;
static VALUE rb_c_cogl_texture;

typedef struct _PolygonData PolygonData;

struct _PolygonData
{
  int argc;
  VALUE *argv;
  VALUE self;
  CoglTextureVertex *vertices;
};

static int
rb_cogl_texture_get_format_bpp (CoglPixelFormat format)
{
  int bpp = 0;

  format &= COGL_UNORDERED_MASK;

  if (format == COGL_PIXEL_FORMAT_A_8
      || format == COGL_PIXEL_FORMAT_G_8)
    bpp = 1;
  else if (format == COGL_PIXEL_FORMAT_RGB_565
	   || format == COGL_PIXEL_FORMAT_RGBA_4444
	   || format == COGL_PIXEL_FORMAT_RGBA_5551)
    bpp = 2;
  else if (format == COGL_PIXEL_FORMAT_24)
    bpp = 3;
  else if (format == COGL_PIXEL_FORMAT_32)
    bpp = 4;
  else
    rb_raise (rb_c_cogl_texture_error, "no rowstride given "
	      "and unknown pixel format used");

  return bpp;
}

static gint
rb_cogl_texture_max_waste_param (VALUE value)
{
  return NIL_P (value) ? 64 : NUM2INT (value);
}

static VALUE
rb_cogl_texture_initialize (int argc, VALUE *argvin, VALUE self)
{
  VALUE argv[8];
  GError *error = NULL;
  CoglHandle tex = COGL_INVALID_HANDLE;

  argc = rb_scan_args (argc, argvin, "17", argv, argv + 1, argv + 2,
		       argv + 3, argv + 4, argv + 5, argv + 6, argv + 7);

  if (argc <= 4)
    tex = cogl_texture_new_from_file (StringValuePtr (argv[0]),
				      rb_cogl_texture_max_waste_param (argv[1]),
				      RTEST (argv[2]),
				      argv[3] == Qnil
				      ? COGL_PIXEL_FORMAT_ANY
				      : NUM2INT (argv[3]),
				      &error);
  else if (argc == 5)
    tex = cogl_texture_new_with_size (NUM2UINT (argv[0]),
				      NUM2UINT (argv[1]),
				      rb_cogl_texture_max_waste_param (argv[2]),
				      RTEST (argv[3]),
				      NUM2UINT (argv[4]));
  else if (argc == 8)
    {
      guint width = NUM2UINT (argv[0]);
      guint height = NUM2UINT (argv[1]);
      gint max_waste = rb_cogl_texture_max_waste_param (argv[2]);
      guint rowstride;
      const char *data = StringValuePtr (argv[7]);

      /* If there is no rowstride then try to guess what it will be
	 from the format */
      if (NIL_P (argv[6]) || (rowstride = NUM2UINT (argv[6])) == 0)
	rowstride = width * rb_cogl_texture_get_format_bpp (NUM2UINT (argv[4]));

      /* Make sure the string is long enough */
      if (RSTRING_LEN (argv[7]) < height * rowstride)
	rb_raise (rb_eArgError, "data string too short");

      tex = cogl_texture_new_from_data (width, height, max_waste,
					RTEST (argv[3]),
					NUM2UINT (argv[4]),
					argv[5] == Qnil
					? COGL_PIXEL_FORMAT_ANY
					: NUM2INT (argv[5]),
					rowstride,
					(const guchar *) data);
    }
  else
    rb_raise (rb_eArgError, "wrong number of arguments");

  if (error)
    RAISE_GERROR (error);
  else if (tex == COGL_INVALID_HANDLE)
    rb_raise (rb_c_cogl_texture_error, "Cogl texture creation failed");

  G_INITIALIZE (self, tex);
  
  cogl_texture_unref (tex);

  return Qnil;
}

CoglHandle
rb_cogl_texture_get_handle (VALUE obj)
{
  return (CoglHandle) RVAL2BOXED (obj, CLUTTER_TYPE_TEXTURE_HANDLE);
}

static VALUE
rb_cogl_texture_get_width (VALUE self)
{
  CoglHandle tex = rb_cogl_texture_get_handle (self);

  return UINT2NUM (cogl_texture_get_width (tex));
}

static VALUE
rb_cogl_texture_get_height (VALUE self)
{
  CoglHandle tex = rb_cogl_texture_get_handle (self);

  return UINT2NUM (cogl_texture_get_height (tex));
}

static VALUE
rb_cogl_texture_get_format (VALUE self)
{
  CoglHandle tex = rb_cogl_texture_get_handle (self);

  return UINT2NUM (cogl_texture_get_format (tex));
}

static VALUE
rb_cogl_texture_get_rowstride (VALUE self)
{
  CoglHandle tex = rb_cogl_texture_get_handle (self);

  return UINT2NUM (cogl_texture_get_rowstride (tex));
}

static VALUE
rb_cogl_texture_get_max_waste (VALUE self)
{
  CoglHandle tex = rb_cogl_texture_get_handle (self);

  return INT2NUM (cogl_texture_get_max_waste (tex));
}

static VALUE
rb_cogl_texture_get_min_filter (VALUE self)
{
  CoglHandle tex = rb_cogl_texture_get_handle (self);

  return UINT2NUM (cogl_texture_get_min_filter (tex));
}

static VALUE
rb_cogl_texture_get_mag_filter (VALUE self)
{
  CoglHandle tex = rb_cogl_texture_get_handle (self);

  return UINT2NUM (cogl_texture_get_mag_filter (tex));
}

static VALUE
rb_cogl_texture_is_sliced (VALUE self)
{
  CoglHandle tex = rb_cogl_texture_get_handle (self);

  return cogl_texture_is_sliced (tex) ? Qtrue : Qfalse;
}

static VALUE
rb_cogl_texture_rectangle (int argc, VALUE *argv, VALUE self)
{
  VALUE x1in, y1in, x2in, y2in, tx1, ty1, tx2, ty2;
  ClutterFixed x1, y1, x2, y2;
  CoglHandle tex = rb_cogl_texture_get_handle (self);
  
  rb_scan_args (argc, argv, "26", &x1in, &y1in, &x2in, &y2in,
		&tx1, &ty1, &tx2, &ty2);

  x1 = rbclt_num_to_fixed (x1in);
  y1 = rbclt_num_to_fixed (y1in);

  if (x2in == Qnil)
    x2 = CLUTTER_INT_TO_FIXED (cogl_texture_get_width (tex))
      + x1;
  else
    x2 = rbclt_num_to_fixed (x2in);

  if (y2in == Qnil)
    y2 = CLUTTER_INT_TO_FIXED (cogl_texture_get_height (tex))
      + y1;
  else
    y2 = rbclt_num_to_fixed (y2in);

  cogl_texture_rectangle (tex,
			  x1, y1, x2, y2,
			  tx1 == Qnil
			  ? 0 : rbclt_num_to_fixed (tx1),
			  ty1 == Qnil
			  ? 0 : rbclt_num_to_fixed (ty1),
			  tx2 == Qnil
			  ? CFX_ONE : rbclt_num_to_fixed (tx2),
			  ty2 == Qnil
			  ? CFX_ONE : rbclt_num_to_fixed (ty2));
  
  return self;
}

static VALUE
rb_cogl_texture_get_data (int argc, VALUE *argv, VALUE self)
{
  CoglHandle tex = rb_cogl_texture_get_handle (self);
  VALUE format_arg, rowstride_arg;
  CoglPixelFormat format;
  guint rowstride;
  VALUE data;

  rb_scan_args (argc, argv, "02", &format_arg, &rowstride_arg);

  format = NIL_P (format_arg)
    ? cogl_texture_get_format (tex) : NUM2UINT (format_arg);
  rowstride = (NIL_P (rowstride_arg) || NUM2UINT (rowstride_arg) == 0)
    ? cogl_texture_get_rowstride (tex) : NUM2UINT (rowstride_arg);

  data = rb_str_buf_new (rowstride * cogl_texture_get_height (tex));
  cogl_texture_get_data (tex, format, rowstride,
			 (guchar *) RSTRING (data)->ptr);

  RSTRING (data)->len = rowstride * cogl_texture_get_height (tex);

  return data;
}

static VALUE
rb_cogl_texture_set_min_filter (VALUE self, VALUE min_filter)
{
  CoglHandle tex = rb_cogl_texture_get_handle (self);

  cogl_texture_set_filters (tex, NUM2UINT (min_filter),
			    cogl_texture_get_mag_filter (tex));

  return self;
}

static VALUE
rb_cogl_texture_set_mag_filter (VALUE self, VALUE mag_filter)
{
  CoglHandle tex = rb_cogl_texture_get_handle (self);

  cogl_texture_set_filters (tex,
			    cogl_texture_get_min_filter (tex),
			    NUM2UINT (mag_filter));

  return self;
}

static VALUE
rb_cogl_texture_set_filters (VALUE self, VALUE min_filter, VALUE mag_filter)
{
  CoglHandle tex = rb_cogl_texture_get_handle (self);

  cogl_texture_set_filters (tex, NUM2UINT (min_filter), NUM2UINT (mag_filter));

  return self;
}

static VALUE
rb_cogl_texture_set_region (VALUE self, VALUE src_x, VALUE src_y,
			    VALUE dst_x, VALUE dst_y,
			    VALUE dst_width, VALUE dst_height,
			    VALUE width_arg, VALUE height_arg,
			    VALUE format, VALUE rowstride_arg,
			    VALUE data_arg)
{
  guint width = NUM2UINT (width_arg);
  guint height = NUM2UINT (height_arg);
  guint rowstride;
  const char *data = StringValuePtr (data_arg);

  /* If there is no rowstride then try to guess what it will be from
     the format */
  if (NIL_P (rowstride_arg) || (rowstride = NUM2UINT (rowstride_arg)) == 0)
    rowstride = width * rb_cogl_texture_get_format_bpp (NUM2UINT (format));

  /* Make sure the string is long enough */
  if (RSTRING_LEN (data_arg) < height * rowstride)
    rb_raise (rb_eArgError, "data string too short");

  if (!cogl_texture_set_region (rb_cogl_texture_get_handle (self),
				NUM2INT (src_x), NUM2INT (src_y),
				NUM2INT (dst_x), NUM2INT (dst_y),
				NUM2UINT (dst_width), NUM2UINT (dst_height),
				width, height,
				NUM2UINT (format),
				rowstride,
				(const guchar *) data))
    rb_raise (rb_c_cogl_texture_error, "texture set region failed");

  return self;
}

static VALUE
rb_cogl_texture_do_polygon (PolygonData *data)
{
  int i;
  gboolean use_color = FALSE;

  for (i = 0; i < data->argc; i++)
    {
      struct RArray *array;

      Check_Type (data->argv[i], T_ARRAY);

      array = RARRAY (data->argv[i]);

      switch (array->len)
	{
	default:
	  rb_raise (rb_eArgError, "array too long in argument %i", i + 1);
	  break;

	case 6:
	  data->vertices[i].color
	    = *(ClutterColor *) RVAL2BOXED (array->ptr[5], CLUTTER_TYPE_COLOR);
	  use_color = TRUE;
	case 5:
	  data->vertices[i].ty
	    = rbclt_num_to_fixed (array->ptr[4]);
	case 4:
	  data->vertices[i].tx
	    = rbclt_num_to_fixed (array->ptr[3]);
	case 3:
	  data->vertices[i].z
	    = rbclt_num_to_fixed (array->ptr[2]);
	case 2:
	  data->vertices[i].y
	    = rbclt_num_to_fixed (array->ptr[1]);
	case 1:
	  data->vertices[i].x
	    = rbclt_num_to_fixed (array->ptr[0]);
	case 0:
	  break;
	}
    }

  cogl_texture_polygon (rb_cogl_texture_get_handle (data->self),
			data->argc, data->vertices, use_color);

  return data->self;
}

static VALUE
rb_cogl_texture_free_polygon_data (PolygonData *data)
{
  free (data->vertices);

  return Qnil;
}

static VALUE
rb_cogl_texture_polygon (int argc, VALUE *argv, VALUE self)
{
  PolygonData data;

  data.argc = argc;
  data.argv = argv;
  data.self = self;
  data.vertices = ALLOC_N (CoglTextureVertex, argc);

  return rb_ensure (rb_cogl_texture_do_polygon,
		    (VALUE) &data,
		    rb_cogl_texture_free_polygon_data,
		    (VALUE) &data);
}

void
rb_cogl_texture_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_TEXTURE_HANDLE, "Texture",
			     rbclt_c_cogl);
  rb_c_cogl_texture = klass;

  rb_c_cogl_texture_error = rb_define_class_under (klass, "Error",
						   rb_eStandardError);

  rb_define_method (klass, "initialize", rb_cogl_texture_initialize, -1);
  rb_define_method (klass, "rectangle", rb_cogl_texture_rectangle, -1);
  rb_define_method (klass, "width", rb_cogl_texture_get_width, 0);
  rb_define_method (klass, "height", rb_cogl_texture_get_height, 0);
  rb_define_method (klass, "format", rb_cogl_texture_get_format, 0);
  rb_define_method (klass, "rowstride", rb_cogl_texture_get_rowstride, 0);
  rb_define_method (klass, "max_waste", rb_cogl_texture_get_max_waste, 0);
  rb_define_method (klass, "min_filter", rb_cogl_texture_get_min_filter, 0);
  rb_define_method (klass, "mag_filter", rb_cogl_texture_get_mag_filter, 0);
  rb_define_method (klass, "sliced?", rb_cogl_texture_is_sliced, 0);
  rb_define_method (klass, "get_data", rb_cogl_texture_get_data, -1);
  rb_define_alias (klass, "data", "get_data");
  rb_define_method (klass, "set_min_filter",
		    rb_cogl_texture_set_min_filter, 1);
  rb_define_method (klass, "set_mag_filter",
		    rb_cogl_texture_set_mag_filter, 1);
  rb_define_method (klass, "set_filters",
		    rb_cogl_texture_set_filters, 2);
  rb_define_method (klass, "set_region",
		    rb_cogl_texture_set_region, 11);
  rb_define_method (klass, "polygon",
		    rb_cogl_texture_polygon, -1);

  G_DEF_SETTERS (klass);
}
