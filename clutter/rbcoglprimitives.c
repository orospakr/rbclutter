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

#include "rbclutter.h"

typedef struct _PolyData PolyData;

struct _PolyData
{
  int argc;
  VALUE *argv;
  ClutterFixed *points;
  void (* func) (ClutterFixed *coords, gint num_points);
};

static VALUE
rb_cogl_color (int argc, VALUE *argv, VALUE self)
{
  /* Accept either a single ClutterColor argument or up to four
     components */
  if (argc == 1
      && RTEST (rb_obj_is_kind_of (argv[0], GTYPE2CLASS (CLUTTER_TYPE_COLOR))))
    {
      const ClutterColor *color
        = (const ClutterColor *) RVAL2BOXED (argv[0], CLUTTER_TYPE_COLOR);

      cogl_color (color);
    }
  else
    {
      VALUE r, g, b, a;
      ClutterColor color;

      rb_scan_args (argc, argv, "04", &r, &g, &b, &a);

      color.red = NIL_P (r) ? 0 : rbclt_num_to_guint8 (r);
      color.green = NIL_P (g) ? 0 : rbclt_num_to_guint8 (g);
      color.blue = NIL_P (b) ? 0 : rbclt_num_to_guint8 (b);
      color.alpha = NIL_P (a) ? 255 : rbclt_num_to_guint8 (a);

      cogl_color (&color);
    }

  return Qnil;
}

static VALUE
rb_cogl_rectangle (VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
  /* If all of the values are fixed nums then use the integer
     version */
  if (FIXNUM_P (x) && FIXNUM_P (y) && FIXNUM_P (width) && FIXNUM_P (height))
    cogl_rectangle (FIX2LONG (x), FIX2LONG (y),
                    FIX2ULONG (width), FIX2ULONG (height));
  else
    cogl_rectanglex (rbclt_num_to_fixed (x),
                     rbclt_num_to_fixed (y),
                     rbclt_num_to_fixed (width),
                     rbclt_num_to_fixed (height));

  return Qnil;
}

static VALUE
rb_cogl_path_fill (VALUE self)
{
  cogl_path_fill ();

  return Qnil;
}

static VALUE
rb_cogl_path_stroke (VALUE self)
{
  cogl_path_stroke ();

  return Qnil;
}

static VALUE
rb_cogl_path_move_to (VALUE self, VALUE x, VALUE y)
{
  cogl_path_move_to (rbclt_num_to_fixed (x),
                     rbclt_num_to_fixed (y));

  return Qnil;
}

static VALUE
rb_cogl_path_rel_move_to (VALUE self, VALUE x, VALUE y)
{
  cogl_path_rel_move_to (rbclt_num_to_fixed (x),
                         rbclt_num_to_fixed (y));

  return Qnil;
}

static VALUE
rb_cogl_path_line_to (VALUE self, VALUE x, VALUE y)
{
  cogl_path_line_to (rbclt_num_to_fixed (x),
                     rbclt_num_to_fixed (y));

  return Qnil;
}

static VALUE
rb_cogl_path_rel_line_to (VALUE self, VALUE x, VALUE y)
{
  cogl_path_rel_line_to (rbclt_num_to_fixed (x),
                         rbclt_num_to_fixed (y));

  return Qnil;
}

// FIXME: Needs to use CoglFixed instead of ClutterAngle
/* static VALUE
rb_cogl_path_arc (VALUE self,
                  VALUE center_x,
                  VALUE center_y,
                  VALUE radius_x,
                  VALUE radius_y,
                  VALUE angle_1,
                  VALUE angle_2)
{
  cogl_path_arc (rbclt_num_to_fixed (center_x),
                 rbclt_num_to_fixed (center_y),
                 rbclt_num_to_fixed (radius_x),
                 rbclt_num_to_fixed (radius_y),
                 rbclt_num_to_angle (angle_1),
                 rbclt_num_to_angle (angle_2));

  return Qnil;
}
*/

static VALUE
rb_cogl_path_curve_to (VALUE self,
                       VALUE x1,
                       VALUE y1,
                       VALUE x2,
                       VALUE y2,
                       VALUE x3,
                       VALUE y3)
{
  cogl_path_curve_to (rbclt_num_to_fixed (x1),
                      rbclt_num_to_fixed (y1),
                      rbclt_num_to_fixed (x2),
                      rbclt_num_to_fixed (y2),
                      rbclt_num_to_fixed (x3),
                      rbclt_num_to_fixed (y3));

  return Qnil;
}

static VALUE
rb_cogl_path_rel_curve_to (VALUE self,
                           VALUE x1,
                           VALUE y1,
                           VALUE x2,
                           VALUE y2,
                           VALUE x3,
                           VALUE y3)
{
  cogl_path_rel_curve_to (rbclt_num_to_fixed (x1),
                          rbclt_num_to_fixed (y1),
                          rbclt_num_to_fixed (x2),
                          rbclt_num_to_fixed (y2),
                          rbclt_num_to_fixed (x3),
                          rbclt_num_to_fixed (y3));

  return Qnil;
}

static VALUE
rb_cogl_path_close (VALUE self)
{
  cogl_path_close ();

  return Qnil;
}

static VALUE
rb_cogl_path_line (VALUE self,
                   VALUE x1,
                   VALUE y1,
                   VALUE x2,
                   VALUE y2)
{
  cogl_path_line (rbclt_num_to_fixed (x1),
                  rbclt_num_to_fixed (y1),
                  rbclt_num_to_fixed (x2),
                  rbclt_num_to_fixed (y2));

  return Qnil;
}

static VALUE
rb_cogl_path_rectangle (VALUE self,
                        VALUE x,
                        VALUE y,
                        VALUE width,
                        VALUE height)
{
  cogl_path_rectangle (rbclt_num_to_fixed (x),
                       rbclt_num_to_fixed (y),
                       rbclt_num_to_fixed (width),
                       rbclt_num_to_fixed (height));

  return Qnil;
}

static VALUE
rb_cogl_path_ellipse (VALUE self,
                      VALUE center_x,
                      VALUE center_y,
                      VALUE radius_x,
                      VALUE radius_y)
{
  cogl_path_ellipse (rbclt_num_to_fixed (center_x),
                     rbclt_num_to_fixed (center_y),
                     rbclt_num_to_fixed (radius_x),
                     rbclt_num_to_fixed (radius_y));

  return Qnil;
}

// FIXME: Needs to use CoglFixed rather than ClutterAngle.
/* static VALUE */
/* rb_cogl_path_round_rectangle (VALUE self, */
/*                               VALUE x, */
/*                               VALUE y, */
/*                               VALUE width, */
/*                               VALUE height, */
/*                               VALUE radius, */
/*                               VALUE arc_step) */
/* { */
/*   cogl_path_round_rectangle (rbclt_num_to_fixed (x), */
/*                              rbclt_num_to_fixed (y), */
/*                              rbclt_num_to_fixed (width), */
/*                              rbclt_num_to_fixed (height), */
/*                              rbclt_num_to_fixed (radius), */
/*                              rbclt_num_to_angle (arc_step)); */

/*   return Qnil; */
/* } */

static VALUE
rb_cogl_do_poly_func (VALUE data_value)
{
  PolyData *data = (PolyData *) data_value;
  int i;

  for (i = 0; i < data->argc; i++)
    data->points[i] = rbclt_num_to_fixed (data->argv[i]);

  (* data->func) (data->points, data->argc / 2);

  return Qnil;
}

static VALUE
rb_cogl_free_poly_data (VALUE data_value)
{
  free (((PolyData *) data_value)->points);

  return Qnil;
}

static VALUE
rb_cogl_poly_func (PolyData *data)
{
  if ((data->argc & 1))
    rb_raise (rb_eArgError, "wrong number of arguments (pairs required)");

  data->points = ALLOC_N (ClutterFixed, data->argc);

  return rb_ensure (rb_cogl_do_poly_func, (VALUE) data,
                    rb_cogl_free_poly_data, (VALUE) data);
}

static VALUE
rb_cogl_path_polyline (int argc, VALUE *argv, VALUE self)
{
  PolyData data;

  data.argc = argc;
  data.argv = argv;
  data.func = cogl_path_polyline;

  return rb_cogl_poly_func (&data);
}

static VALUE
rb_cogl_path_polygon (int argc, VALUE *argv, VALUE self)
{
  PolyData data;

  data.argc = argc;
  data.argv = argv;
  data.func = cogl_path_polygon;

  return rb_cogl_poly_func (&data);
}

void
rb_cogl_primitives_init ()
{
  rb_define_singleton_method (rbclt_c_cogl, "color", rb_cogl_color, -1);
  rb_define_singleton_method (rbclt_c_cogl, "rectangle", rb_cogl_rectangle, 4);
  rb_define_singleton_method (rbclt_c_cogl, "path_fill", rb_cogl_path_fill, 0);
  rb_define_singleton_method (rbclt_c_cogl, "path_stroke",
                              rb_cogl_path_stroke, 0);
  rb_define_singleton_method (rbclt_c_cogl, "path_move_to",
                              rb_cogl_path_move_to, 2);
  rb_define_singleton_method (rbclt_c_cogl, "path_rel_move_to",
                              rb_cogl_path_rel_move_to, 2);
  rb_define_singleton_method (rbclt_c_cogl, "path_line_to",
                              rb_cogl_path_line_to, 2);
  rb_define_singleton_method (rbclt_c_cogl, "path_rel_line_to",
                              rb_cogl_path_rel_line_to, 2);
  rb_define_singleton_method (rbclt_c_cogl, "path_arc",
                              rb_cogl_path_arc, 6);
  rb_define_singleton_method (rbclt_c_cogl, "path_curve_to",
                              rb_cogl_path_curve_to, 6);
  rb_define_singleton_method (rbclt_c_cogl, "path_rel_curve_to",
                              rb_cogl_path_rel_curve_to, 6);
  rb_define_singleton_method (rbclt_c_cogl, "path_close",
                              rb_cogl_path_close, 0);
  rb_define_singleton_method (rbclt_c_cogl, "path_line",
                              rb_cogl_path_line, 4);
  rb_define_singleton_method (rbclt_c_cogl, "path_rectangle",
                              rb_cogl_path_rectangle, 4);
  rb_define_singleton_method (rbclt_c_cogl, "path_ellipse",
                              rb_cogl_path_ellipse, 4);
  rb_define_singleton_method (rbclt_c_cogl, "path_round_rectangle",
                              rb_cogl_path_round_rectangle, 6);
  rb_define_singleton_method (rbclt_c_cogl, "path_polyline",
                              rb_cogl_path_polyline, -1);
  rb_define_singleton_method (rbclt_c_cogl, "path_polygon",
                              rb_cogl_path_polygon, -1);
}
