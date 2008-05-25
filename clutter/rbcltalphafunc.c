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
#include <clutter/clutter-alpha.h>

#include "rbcltalphafunc.h"

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

#define RBCLT_ALPHA_FUNC_MAP_COUNT (sizeof (rbclt_alpha_func_map)	\
				    / sizeof (rbclt_alpha_func_map[0]))

static ID id_call = 0;

/* Wrapper so that we can store the proc value in a global variable
   and register it with Ruby */
typedef struct _AlphaFuncWrapper AlphaFuncWrapper;

struct _AlphaFuncWrapper
{
  /* The proc to call */
  VALUE proc;
};

static guint32
rbclt_alpha_func_wrapper_call (ClutterAlpha *alpha, gpointer user_data)
{
  AlphaFuncWrapper *wrapper = (AlphaFuncWrapper *) user_data;
  return NUM2UINT (rb_funcall (wrapper->proc, id_call, 1, GOBJ2RVAL (alpha)));
}

static void
rbclt_alpha_func_wrapper_destroy (gpointer data)
{
  AlphaFuncWrapper *wrapper = (AlphaFuncWrapper *) data;
  rb_gc_unregister_address (&wrapper->proc);
  g_slice_free (AlphaFuncWrapper, wrapper);
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
      AlphaFuncWrapper *wrapper = g_slice_new (AlphaFuncWrapper);
      wrapper->proc = func;
      rb_gc_register_address (&wrapper->proc);
      *func_ret = rbclt_alpha_func_wrapper_call;
      *data = wrapper;
      *notify = rbclt_alpha_func_wrapper_destroy;
    }
}

void
rbclt_alpha_func_init (VALUE alpha_klass)
{
  int i;

  id_call = rb_intern ("call");

  for (i = 0; i < RBCLT_ALPHA_FUNC_MAP_COUNT; i++)
    rb_define_const (alpha_klass, rbclt_alpha_func_map[i].name, INT2FIX (i));
}
