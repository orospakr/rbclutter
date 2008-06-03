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

#include <clutter/clutter-main.h>
#include <clutter/clutter-enum-types.h>
#include <rbgobject.h>

#include "rbclutter.h"
#include "rbcltstagemanager.h"

static VALUE rbclt_c_clutter_init_error = Qnil;

static VALUE
rbclt_init (int argc, VALUE *argv)
{
  VALUE init_args, prg_name;
  char **args_copy;
  int init_argc, i;
  ClutterInitError eval;

  rb_scan_args (argc, argv, "01", &init_args);

  /* Default to using ARGV if the argument list wasn't given */
  if (init_args == Qnil)
    init_args = rb_argv;

  Check_Type (init_args, T_ARRAY);
  init_argc = RARRAY (init_args)->len;

  /* Convert all of the arguments to strings */
  for (i = 0; i < init_argc; i++)
    rb_ary_store (init_args, i, StringValue (RARRAY (init_args)->ptr[i]));

  /* Make a copy of the array and prepend $0 */
  prg_name = rb_gv_get ("0");
  prg_name = StringValue (prg_name);
  args_copy = ALLOC_N (char *, init_argc + 1);
  args_copy[0] = RSTRING (prg_name)->ptr;
  for (i = 0; i < init_argc; i++)
    args_copy[i + 1] = RSTRING (RARRAY (init_args)->ptr[i])->ptr;

  /* Initialize clutter */
  init_argc++;
  eval = clutter_init (&init_argc, &args_copy);
  
  /* If it worked then copy the altered arguments back */
  if (eval == CLUTTER_INIT_SUCCESS)
    {
      rb_ary_clear (init_args);

      for (i = 1; i < init_argc; i++)
	rb_ary_push (init_args, rb_str_new2 (args_copy[i]));
    }

  free (args_copy);

  /* If it didn't work then throw an exception */
  if (eval != CLUTTER_INIT_SUCCESS)
    {
      VALUE ex = rb_exc_new2 (rbclt_c_clutter_init_error, "Failed to initalise Clutter");
      rb_iv_set (ex, "@errnum", GENUM2RVAL (eval, CLUTTER_TYPE_INIT_ERROR));
      rb_exc_raise (ex);
    }

  rbclt_stage_manager_init_global_mark ();

  return Qnil;
}

static VALUE
rbclt_get_debug_enabled ()
{
  return clutter_get_debug_enabled () ? Qtrue : Qfalse;
}

static VALUE
rbclt_get_show_fps ()
{
  return clutter_get_show_fps () ? Qtrue : Qfalse;
}

static VALUE
rbclt_get_timestamp ()
{
  return ULONG2NUM (clutter_get_timestamp ());
}

static VALUE
rbclt_main ()
{
  clutter_main ();
  return Qnil;
}

static VALUE
rbclt_main_quit ()
{
  clutter_main_quit ();
  return Qnil;
}

static VALUE
rbclt_main_level ()
{
  return INT2NUM (clutter_main_level ());
}

void
rbclt_main_init ()
{
  rb_define_module_function (rbclt_c_clutter, "init", rbclt_init, -1);

  rbclt_c_clutter_init_error = rb_define_class_under (rbclt_c_clutter,
						      "InitError",
						      rbclt_c_clutter_error);
  rb_define_attr (rbclt_c_clutter_init_error, "errnum", Qtrue, Qfalse);
  G_DEF_CLASS (CLUTTER_TYPE_INIT_ERROR, "Code", rbclt_c_clutter_init_error);
  G_DEF_CONSTANTS (rbclt_c_clutter_init_error, CLUTTER_TYPE_INIT_ERROR, "CLUTTER_INIT_");

  rb_define_module_function (rbclt_c_clutter, "debug_enabled?", rbclt_get_debug_enabled, 0);
  rb_define_module_function (rbclt_c_clutter, "show_fps?", rbclt_get_show_fps, 0);
  rb_define_module_function (rbclt_c_clutter, "timestamp", rbclt_get_timestamp, 0);
  rb_define_module_function (rbclt_c_clutter, "main", rbclt_main, 0);
  rb_define_module_function (rbclt_c_clutter, "main_quit", rbclt_main_quit, 0);
  rb_define_module_function (rbclt_c_clutter, "main_level", rbclt_main_level, 0);
}
