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
#include <clutter/clutter-effect.h>
#include <clutter/clutter-version.h>
#include <clutter/clutter-types.h>
#include <clutter/clutter-enum-types.h>

#include "rbclutter.h"
#include "rbcltalphafunc.h"

static VALUE
rbclt_effect_template_initialize (int argc, VALUE *argv, VALUE self)
{
  VALUE timeline, func;
  ClutterEffectTemplate *et;
  ClutterAlphaFunc func_ptr;
  gpointer data;
  GDestroyNotify notify;

  rb_scan_args (argc, argv, "11", &timeline, &func);

  rbclt_alpha_func_from_rb_value (func, &func_ptr, &data, &notify);

  et = clutter_effect_template_new_full (RVAL2GOBJ (timeline),
					 func_ptr, data, notify);

  G_INITIALIZE (self, et);

  return Qnil;
}

static VALUE
rbclt_effect_fade (VALUE self, VALUE actor, VALUE start_opacity, VALUE end_opacity)
{
  ClutterEffectTemplate *effect = CLUTTER_EFFECT_TEMPLATE (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_effect_fade (effect, CLUTTER_ACTOR (RVAL2GOBJ (actor)),
					 rbclt_num_to_guint8 (start_opacity),
					 rbclt_num_to_guint8 (end_opacity),
					 NULL, NULL));
}

struct RbcltEffectMoveData
{
  VALUE self, actor, knots;
  ClutterKnot *knot_array;
};

static VALUE
rbclt_effect_do_move (VALUE arg)
{
  struct RbcltEffectMoveData *data = (struct RbcltEffectMoveData *) arg;
  int i;

  for (i = 0; i < RARRAY (data->knots)->len; i++)
    data->knot_array[i] = *(ClutterKnot *) RVAL2BOXED (RARRAY (data->knots)->ptr[i],
						       CLUTTER_TYPE_KNOT);

  return GOBJ2RVAL (clutter_effect_move (CLUTTER_EFFECT_TEMPLATE (RVAL2GOBJ (data->self)),
					 CLUTTER_ACTOR (RVAL2GOBJ (data->actor)),
					 data->knot_array,
					 RARRAY (data->knots)->len,
					 NULL, NULL));
}

static VALUE
rbclt_effect_free_param (VALUE param)
{
  free ((void *) param);
  return Qnil;
}

static VALUE
rbclt_effect_move (int argc, VALUE *argv, VALUE self)
{
  struct RbcltEffectMoveData data;

  data.self = self;
  rb_scan_args (argc, argv, "10*", &data.actor, &data.knots);
  data.knot_array = ALLOC_N (ClutterKnot, RARRAY (data.knots)->len);

  /* Call the rest of the code under rb_ensure so that we can be sure
     the knot_array is freed even if an exception is thrown (for
     example if one of the knots isn't really a knot) */
  return rb_ensure (rbclt_effect_do_move, (VALUE) &data,
		    rbclt_effect_free_param, (VALUE) data.knot_array);
}

static VALUE
rbclt_effect_scale (VALUE self, VALUE actor, VALUE scale_begin, VALUE scale_end, VALUE gravity)
{
  ClutterEffectTemplate *effect = CLUTTER_EFFECT_TEMPLATE (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_effect_scale (effect, CLUTTER_ACTOR (RVAL2GOBJ (actor)),
					  NUM2DBL (scale_begin),
					  NUM2DBL (scale_end),
					  RVAL2GENUM (gravity, CLUTTER_TYPE_GRAVITY),
					  NULL, NULL));
}

#define rbclt_effect_rotate_func(axis)					\
  static VALUE								\
  rbclt_effect_rotate_ ## axis (VALUE self, VALUE actor,		\
				VALUE angle_begin, VALUE angle_end,	\
				VALUE center1, VALUE center2,		\
				VALUE direction)			\
  {									\
    ClutterEffectTemplate *effect =					\
      CLUTTER_EFFECT_TEMPLATE (RVAL2GOBJ (self));			\
    return GOBJ2RVAL (clutter_effect_rotate_ ## axis			\
		      (effect, CLUTTER_ACTOR (RVAL2GOBJ (actor)),	\
		       NUM2DBL (angle_begin),				\
		       NUM2DBL (angle_end),				\
		       NUM2INT (center1),				\
		       NUM2INT (center2),				\
		       RVAL2GENUM (direction,				\
				   CLUTTER_TYPE_ROTATE_DIRECTION),	\
		       NULL, NULL));					\
  }

rbclt_effect_rotate_func(x)
rbclt_effect_rotate_func(y)
rbclt_effect_rotate_func(z)

void
rbclt_effect_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_EFFECT_TEMPLATE, "EffectTemplate", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_effect_template_initialize, -1);
  rb_define_method (klass, "fade", rbclt_effect_fade, 3);
  rb_define_method (klass, "move", rbclt_effect_move, -1);
  rb_define_method (klass, "scale", rbclt_effect_scale, 4);
  rb_define_method (klass, "rotate_x", rbclt_effect_rotate_x, 6);
  rb_define_method (klass, "rotate_y", rbclt_effect_rotate_y, 6);
  rb_define_method (klass, "rotate_z", rbclt_effect_rotate_z, 6);
}
