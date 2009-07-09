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
#include <ruby.h>
#include <clutter/clutter.h>

#include "rbcltalphafunc.h"
#include "rbcltcallbackfunc.h"

static struct { const char *name; ClutterAlphaFunc func; }
  rbclt_alpha_func_map[] =
    {
      { "RAMP_INC", clutter_ramp_inc_func },
      { "RAMP_DEC", clutter_ramp_dec_func },
      { "RAMP", clutter_ramp_func },
      { "SINE", clutter_sine_func },
      { "SINE_INC", clutter_sine_inc_func },
      { "SINE_DEC", clutter_sine_dec_func },
      { "SINE_HALF", clutter_sine_half_func },
      { "SQUARE", clutter_square_func },
      { "SMOOTHSTEP_INC", clutter_smoothstep_inc_func },
      { "SMOOTHSTEP_DEC", clutter_smoothstep_dec_func },
      { "EXP_INC", clutter_exp_inc_func },
      { "EXP_DEC", clutter_exp_dec_func }
    };

#define RBCLT_ALPHA_FUNC_MAP_COUNT (sizeof (rbclt_alpha_func_map)       \
                                    / sizeof (rbclt_alpha_func_map[0]))

static guint32
rbclt_alpha_func_wrapper_call (ClutterAlpha *alpha, gpointer user_data)
{
  RBCLTCallbackFunc *func = (RBCLTCallbackFunc *) user_data;
  VALUE arg = GOBJ2RVAL (alpha);

  return NUM2UINT (rbclt_callback_func_invoke (func, 1, &arg));
}

void
rbclt_alpha_func_from_rb_value (VALUE func,
                                ClutterAlphaFunc *func_ret,
                                gpointer *data,
                                GDestroyNotify *notify)
{
  /* If there is no argument then get the function from the block */
  if (func == Qnil)
    {
      rb_need_block ();
      func = rb_block_proc ();
    }

  /* If the function is an integer constant representing a builtin
     clutter function then use the function pointer directly
     instead */
  if (FIXNUM_P (func) && FIX2INT (func) >= 0 && FIX2INT (func) < RBCLT_ALPHA_FUNC_MAP_COUNT)
    {
      *func_ret = rbclt_alpha_func_map[FIX2INT (func)].func;
      *data = NULL;
      *notify = NULL;
    }
  else
    {
      *func_ret = rbclt_alpha_func_wrapper_call;
      *data = rbclt_callback_func_new (func);
      *notify = (GDestroyNotify) rbclt_callback_func_destroy;
    }
}

void
rbclt_alpha_func_init (VALUE alpha_klass)
{
  int i;

  for (i = 0; i < RBCLT_ALPHA_FUNC_MAP_COUNT; i++)
    rb_define_const (alpha_klass, rbclt_alpha_func_map[i].name, INT2FIX (i));
}
