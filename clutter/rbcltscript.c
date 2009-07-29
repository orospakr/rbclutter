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
#include <clutter/clutter.h>

#include "rbclutter.h"

static VALUE eNoSignalError;
static VALUE id_call;
static VALUE id_method;

typedef struct _AutoConnectData AutoConnectData;

struct _AutoConnectData
{
  VALUE proc;
  VALUE obj;
  VALUE args;
  int state;

  GObject *object;
  const gchar *signal_name;
  const gchar *handler_name;
  GObject *connect_object;
  GConnectFlags connect_flags;
};

static VALUE
rbclt_script_initialize (VALUE self)
{
  G_INITIALIZE (self, clutter_script_new ());

  return Qnil;
}

static VALUE
rbclt_script_load_from_file (VALUE self, VALUE filename)
{
  ClutterScript *script = CLUTTER_SCRIPT (RVAL2GOBJ (self));
  guint merge_id;
  GError *error = NULL;

  merge_id = clutter_script_load_from_file (script,
                                            StringValuePtr (filename),
                                            &error);

  if (error)
    RAISE_GERROR (error);

  return UINT2NUM (merge_id);
}

static VALUE
rbclt_script_load_from_data (VALUE self, VALUE data)
{
  ClutterScript *script = CLUTTER_SCRIPT (RVAL2GOBJ (self));
  guint merge_id;
  GError *error = NULL;

  StringValuePtr (data);

  merge_id = clutter_script_load_from_data (script,
                                            RSTRING (data)->ptr,
                                            RSTRING (data)->len,
                                            &error);

  if (error)
    RAISE_GERROR (error);

  return UINT2NUM (merge_id);
}

static VALUE
rbclt_script_get_object (int argc, VALUE *argv, VALUE self)
{
  ClutterScript *script = CLUTTER_SCRIPT (RVAL2GOBJ (self));
  GObject *object;

  if (argc == 1)
    {
      object = clutter_script_get_object (script, StringValuePtr (argv[0]));
      return GOBJ2RVAL (object);
    }
  else if (argc > 1)
    {
      VALUE ary = rb_ary_new ();
      int i;

      for (i = 0; i < argc; i++)
        {
          object = clutter_script_get_object (script, StringValuePtr (argv[i]));
          rb_ary_push (ary, GOBJ2RVAL (object));
        }

      return ary;
    }
  else
    {
      rb_raise (rb_eArgError, "wrong number of arguments "
                "(at least one required)");

      return Qnil;
    }
}

static VALUE
rbclt_script_unmerge_objects (VALUE self, VALUE merge_id)
{
  ClutterScript *script = CLUTTER_SCRIPT (RVAL2GOBJ (self));

  clutter_script_unmerge_objects (script, NUM2UINT (merge_id));

  return self;
}

static VALUE
rbclt_script_ensure_objects (VALUE self)
{
  ClutterScript *script = CLUTTER_SCRIPT (RVAL2GOBJ (self));

  clutter_script_ensure_objects (script);

  return self;
}

static VALUE
rbclt_script_get_type_from_name (VALUE self, VALUE name)
{
  ClutterScript *script = CLUTTER_SCRIPT (RVAL2GOBJ (self));
  GType type;

  type = clutter_script_get_type_from_name (script, StringValuePtr (name));

  return type == G_TYPE_INVALID ? Qnil : GTYPE2CLASS (type);
}

static VALUE
rbclt_script_get_script_id (VALUE self, VALUE obj)
{
  const gchar *id = clutter_get_script_id (RVAL2GOBJ (obj));

  if (id == NULL)
    return Qnil;
  else
    return rb_str_new2 (id);
}

static VALUE
rbclt_script_add_search_paths (int argc, VALUE *argv, VALUE self)
{
  ClutterScript *script = CLUTTER_SCRIPT (RVAL2GOBJ (self));
  int i;
  const gchar **paths;

  /* Make sure all of the arguments are strings before allocating the
     array */
  for (i = 0; i < argc; i++)
    StringValuePtr (argv[i]);

  paths = g_malloc (sizeof (const gchar *) * argc);

  for (i = 0; i < argc; i++)
    paths[i] = StringValuePtr (argv[i]);

  clutter_script_add_search_paths (script, paths, argc);

  g_free (paths);

  return self;
}

static VALUE
rbclt_script_lookup_filename (VALUE self, VALUE filename)
{
  ClutterScript *script = CLUTTER_SCRIPT (RVAL2GOBJ (self));
  gchar *full_path;
  VALUE ret;

  full_path = clutter_script_lookup_filename (script,
                                              StringValuePtr (filename));

  if (full_path == NULL)
    ret = Qnil;
  else
    {
      ret = rb_str_new2 (full_path);
      g_free (full_path);
    }

  return ret;
}

static VALUE
rbclt_script_do_connect_signal (VALUE user_data)
{
  AutoConnectData *data = (AutoConnectData *) user_data;
  guint signal_id;
  GQuark detail;
  GClosure *rclosure;

  if (NIL_P (data->proc))
    {
      ID mid;
      VALUE args;
      VALUE func;

      if (!g_signal_parse_name (data->signal_name,
                                G_TYPE_FROM_INSTANCE (data->object),
                                &signal_id, &detail, TRUE))
        rb_raise (eNoSignalError, "no such signal: %s", data->signal_name);

      /* Create a method closure for the given object and method
         name */
      mid = rb_intern (data->handler_name);
      func = rb_funcall (data->obj, id_method, 1,
                         rb_str_new2 (data->handler_name));

      if (data->connect_object)
        args = rb_ary_new3 (1, GOBJ2RVAL (data->connect_object));
      else
        args = data->args;

      rclosure = g_rclosure_new (func, args,
                                 rbgobj_get_signal_func (signal_id));
      g_rclosure_attach ((GClosure *) rclosure, GOBJ2RVAL (data->object));
      g_signal_connect_closure_by_id (data->object, signal_id, detail, rclosure,
                                      (data->connect_flags & G_CONNECT_AFTER)
                                      ? TRUE : FALSE);

    }
  else
    {
      VALUE args;
      int i;

      /* Just pass the details on to the application's function to do
         the actual connection */
      args = rb_ary_new3 (5,
                          GOBJ2RVAL (data->object),
                          rb_str_new2 (data->signal_name),
                          rb_str_new2 (data->handler_name),
                          GOBJ2RVAL (data->connect_object),
                          GENUM2RVAL (data->connect_flags,
                                      rbclt_connect_flags_get_type ()));

      for (i = 0; i < RARRAY_LEN (data->args); i++)
        rb_ary_push (args, RARRAY_PTR (data->args)[i]);

      rb_funcall2 (data->proc, id_call, RARRAY_LEN (args), RARRAY_PTR (args));
    }


  return Qnil;
}

static void
rbclt_script_connect_signal (ClutterScript *script, GObject *object,
                             const gchar *signal_name,
                             const gchar *handler_name,
                             GObject *connect_object,
                             GConnectFlags flags,
                             gpointer user_data)
{
  AutoConnectData *data = (AutoConnectData *) user_data;

  /* Protect the rest of the call so that throwing an exception won't
     cause it to skip the clean up code in
     clutter_script_connect_signals. If there has already been an
     exception then don't do anything but allow the function to
     complete */
  if (data->state == 0)
    {
      data->object = object;
      data->signal_name = signal_name;
      data->handler_name = handler_name;
      data->connect_object = connect_object;
      data->connect_flags = flags;

      rb_protect (rbclt_script_do_connect_signal, (VALUE) user_data,
                  &data->state);
    }
}

static VALUE
rbclt_script_connect_signals (int argc, VALUE *argv, VALUE self)
{
  ClutterScript *script = CLUTTER_SCRIPT (RVAL2GOBJ (self));
  AutoConnectData data;

  data.state = 0;

  if (rb_block_given_p ())
    {
      data.args = rb_ary_new4 (argc, argv);
      data.proc = rb_block_proc ();
      data.obj = Qnil;
    }
  else
    {
      rb_scan_args (argc, argv, "1*", &data.obj, &data.args);
      data.proc = Qnil;
    }

  clutter_script_connect_signals_full (script,
                                       rbclt_script_connect_signal,
                                       &data);

  /* If an exception was thrown then continue throwing that now that
     we are safely out of the Clutter function call */
  if (data.state)
    rb_jump_tag (data.state);

  return self;
}

void
rbclt_script_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_SCRIPT, "Script", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_script_initialize, 0);
  rb_define_method (klass, "load_from_file", rbclt_script_load_from_file, 1);
  rb_define_method (klass, "load_from_data", rbclt_script_load_from_data, 1);
  rb_define_method (klass, "get_object", rbclt_script_get_object, -1);
  rb_define_alias (klass, "get_objects", "get_object");
  rb_define_alias (klass, "[]", "get_object");
  rb_define_method (klass, "unmerge_objects", rbclt_script_unmerge_objects, 1);
  rb_define_method (klass, "ensure_objects", rbclt_script_ensure_objects, 0);
  rb_define_method (klass, "get_type_from_name",
                    rbclt_script_get_type_from_name, 1);
  rb_define_method (klass, "add_search_paths",
                    rbclt_script_add_search_paths, -1);
  rb_define_alias (klass, "add_search_path", "add_search_paths");
  rb_define_method (klass, "lookup_filename", rbclt_script_lookup_filename, 1);
  rb_define_method (klass, "connect_signals", rbclt_script_connect_signals, -1);

  rb_define_singleton_method (rbclt_c_clutter, "get_script_id",
                              rbclt_script_get_script_id, 1);

  G_DEF_ERROR (CLUTTER_SCRIPT_ERROR, "ScriptError", rbclt_c_clutter,
               rb_eRuntimeError, CLUTTER_TYPE_SCRIPT_ERROR);

  id_call = rb_intern ("call");
  id_method = rb_intern ("method");
  eNoSignalError = rb_const_get (rb_const_get (rb_cObject, rb_intern ("GLib")),
                                 rb_intern ("NoSignalError"));

  G_DEF_SETTERS (klass);
}
