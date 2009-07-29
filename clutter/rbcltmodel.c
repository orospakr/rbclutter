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
#include "rbcltcallbackfunc.h"

typedef struct _SetColumnsData SetColumnsData;

struct _SetColumnsData
{
  int argc;
  VALUE *argv;
  VALUE self;
  GType *types;
  const gchar **names;
};

typedef struct _GetData GetData;

struct _GetData
{
  ClutterModelIter *iter;
  guint column;
  GValue value;
  VALUE rvalue;
};

typedef enum
  {
    RBCLT_MODEL_APPEND,
    RBCLT_MODEL_PREPEND,
    RBCLT_MODEL_INSERT,
    RBCLT_MODEL_INSERT_VALUE
  } Action;

typedef struct _ActionData ActionData;

struct _ActionData
{
  Action action;

  int argc;
  VALUE *argv;
  VALUE self;
  guint row;

  guint *colnums;

  GValue *gvalues;
  int gvalues_inited;
};

static VALUE
rbclt_model_free_set_columns_data (VALUE arg)
{
  SetColumnsData *data = (SetColumnsData *) arg;

  if (data->types)
    g_free (data->types);
  if (data->names)
    g_free (data->names);

  return Qnil;
}

static VALUE
rbclt_model_do_set_types (VALUE arg)
{
  SetColumnsData *data = (SetColumnsData *) arg;
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (data->self));
  int i;

  for (i = 0; i < data->argc; i++)
    data->types[i] = CLASS2GTYPE (data->argv[i]);

  clutter_model_set_types (model, data->argc, data->types);

  return data->self;
}

static VALUE
rbclt_model_set_types (int argc, VALUE *argv, VALUE self)
{
  SetColumnsData data;

  if (argc < 1)
    rb_raise (rb_eArgError, "wrong number of arguments "
              "(at least one required)");

  data.argc = argc;
  data.argv = argv;
  data.self = self;
  data.types = ALLOC_N (GType, argc);
  data.names = NULL;

  return rb_ensure (rbclt_model_do_set_types, (VALUE) &data,
                    rbclt_model_free_set_columns_data, (VALUE) &data);
}

static VALUE
rbclt_model_do_set_names (VALUE arg)
{
  SetColumnsData *data = (SetColumnsData *) arg;
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (data->self));
  int i;

  for (i = 0; i < data->argc; i++)
    data->names[i] = StringValuePtr (data->argv[i]);

  clutter_model_set_names (model, data->argc, data->names);

  return data->self;
}

static VALUE
rbclt_model_set_names (int argc, VALUE *argv, VALUE self)
{
  SetColumnsData data;

  if (argc < 1)
    rb_raise (rb_eArgError, "wrong number of arguments "
              "(at least one required)");

  data.argc = argc;
  data.argv = argv;
  data.self = self;
  data.types = NULL;
  data.names = ALLOC_N (const gchar *, argc);

  return rb_ensure (rbclt_model_do_set_names, (VALUE) &data,
                    rbclt_model_free_set_columns_data, (VALUE) &data);
}

static VALUE
rbclt_model_free_action_data (VALUE arg)
{
  ActionData *data = (ActionData *) arg;
  int i;

  for (i = 0; i < data->gvalues_inited; i++)
    g_value_unset (data->gvalues + i);
  g_free (data->gvalues);
  g_free (data->colnums);

  return Qnil;
}

static VALUE
rbclt_model_do_action (VALUE arg)
{
  ActionData *data = (ActionData *) arg;
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (data->self));
  int i;

  for (i = 0; i < data->argc / 2; i++)
    {
      data->colnums[i] = NUM2UINT (data->argv[i * 2]);
      g_value_init (data->gvalues + i, RVAL2GTYPE (data->argv[i * 2 + 1]));
      data->gvalues_inited++;
      rbgobj_rvalue_to_gvalue (data->argv[i * 2 + 1], data->gvalues + i);
    }

  switch (data->action)
    {
    case RBCLT_MODEL_APPEND:
      clutter_model_appendv (model, data->argc / 2, data->colnums,
                             data->gvalues);
      break;

    case RBCLT_MODEL_PREPEND:
      clutter_model_prependv (model, data->argc / 2, data->colnums,
                              data->gvalues);
      break;

    case RBCLT_MODEL_INSERT:
      clutter_model_insertv (model, data->row, data->argc / 2, data->colnums,
                             data->gvalues);
      break;

    case RBCLT_MODEL_INSERT_VALUE:
      clutter_model_insert_value (model, data->row, data->colnums[0],
                                  data->gvalues);
      break;
    }

  return data->self;
}

static VALUE
rbclt_model_action (int argc, VALUE *argv, VALUE self, Action action)
{
  ActionData data;
  VALUE insert_value_args;

  if (action == RBCLT_MODEL_INSERT)
    {
      if (argc < 2)
        rb_raise (rb_eArgError, "wrong number of arguments "
                  "(at least three required)");
      else if ((argc & 1) == 0)
        rb_raise (rb_eArgError, "wrong number of arguments "
                  "(paired arguments required)");

      data.row = NUM2UINT (argv[0]);
      data.argc = argc - 1;
      data.argv = argv + 1;
    }
  else if (action == RBCLT_MODEL_INSERT_VALUE)
    {
      VALUE row;

      rb_scan_args (argc, argv, "1*", &row, &insert_value_args);
      data.row = NUM2UINT (row);
      data.argc = RARRAY_LEN (insert_value_args);
      data.argv = RARRAY_PTR (insert_value_args);
    }
  else
    {
      if (argc < 1)
        rb_raise (rb_eArgError, "wrong number of arguments "
                  "(at least two required)");
      else if ((argc & 1))
        rb_raise (rb_eArgError, "wrong number of arguments "
                  "(paired arguments required)");

      data.argc = argc;
      data.argv = argv;
    }

  data.action = action;
  data.self = self;
  data.colnums = g_malloc (sizeof (guint) * (argc / 2));
  data.gvalues = g_malloc0 (sizeof (GValue) * (argc / 2));
  data.gvalues_inited = 0;

  return rb_ensure (rbclt_model_do_action, (VALUE) &data,
                    rbclt_model_free_action_data, (VALUE) &data);
}

static VALUE
rbclt_model_append (int argc, VALUE *argv, VALUE self)
{
  return rbclt_model_action (argc, argv, self, RBCLT_MODEL_APPEND);
}

static VALUE
rbclt_model_prepend (int argc, VALUE *argv, VALUE self)
{
  return rbclt_model_action (argc, argv, self, RBCLT_MODEL_PREPEND);
}

static VALUE
rbclt_model_insert (int argc, VALUE *argv, VALUE self)
{
  return rbclt_model_action (argc, argv, self, RBCLT_MODEL_INSERT);
}

static VALUE
rbclt_model_insert_value (int argc, VALUE *argv, VALUE self)
{
  return rbclt_model_action (argc, argv, self, RBCLT_MODEL_INSERT_VALUE);
}

static VALUE
rbclt_model_remove (VALUE self, VALUE row)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));

  clutter_model_remove (model, NUM2UINT (row));

  return self;
}

static VALUE
rbclt_model_get_n_rows (VALUE self)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));

  return UINT2NUM (clutter_model_get_n_rows (model));
}

static VALUE
rbclt_model_get_n_columns (VALUE self)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));

  return UINT2NUM (clutter_model_get_n_columns (model));
}

static VALUE
rbclt_model_get_column_name (VALUE self, VALUE column)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));
  const gchar *name;

  name = clutter_model_get_column_name (model, NUM2UINT (column));

  return name ? rb_str_new2 (name) : Qnil;
}

static VALUE
rbclt_model_get_column_type (VALUE self, VALUE colnum)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));

  return GTYPE2CLASS (clutter_model_get_column_type (model, NUM2UINT (colnum)));
}

static VALUE
rbclt_model_get_first_iter (VALUE self)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));

  return GOBJ2RVALU (clutter_model_get_first_iter (model));
}

static VALUE
rbclt_model_get_last_iter (VALUE self)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));

  return GOBJ2RVALU (clutter_model_get_last_iter (model));
}

static VALUE
rbclt_model_get_sorting_column (VALUE self)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));

  return INT2NUM (clutter_model_get_sorting_column (model));
}

static VALUE
rbclt_model_set_sorting_column (VALUE self, VALUE column)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));

  clutter_model_set_sorting_column (model, NUM2UINT (column));

  return self;
}

static gint
rbclt_model_call_sort_func (ClutterModel *model,
                            const GValue *a,
                            const GValue *b,
                            gpointer user_data)
{
  VALUE argv[3];

  argv[0] = GOBJ2RVAL (model);
  argv[1] = GVAL2RVAL (a);
  argv[2] = GVAL2RVAL (b);

  return NUM2INT (rbclt_callback_func_invoke ((RBCLTCallbackFunc *) user_data,
                                              3, argv));
}

static VALUE
rbclt_model_set_sort (VALUE self, VALUE column_arg)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));
  RBCLTCallbackFunc *func;
  guint column = NUM2UINT (column_arg);

  if (rb_block_given_p ())
    {
      func = rbclt_callback_func_new (rb_block_proc ());

      clutter_model_set_sort (model, column,
                              rbclt_model_call_sort_func, func,
                              (GDestroyNotify) rbclt_callback_func_destroy);
    }
  else
    clutter_model_set_sort (model, column, NULL, NULL, NULL);

  return self;
}

static gboolean
rbclt_model_call_filter_func (ClutterModel *model,
                              ClutterModelIter *iter,
                              gpointer user_data)
{
  VALUE argv[2];

  argv[0] = GOBJ2RVAL (model);
  argv[1] = GOBJ2RVAL (iter);

  return RTEST (rbclt_callback_func_invoke ((RBCLTCallbackFunc *) user_data,
                                            2, argv)) ? TRUE : FALSE;
}

static VALUE
rbclt_model_set_filter (VALUE self)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));
  RBCLTCallbackFunc *func;

  if (rb_block_given_p ())
    {
      func = rbclt_callback_func_new (rb_block_proc ());

      clutter_model_set_filter (model, rbclt_model_call_filter_func, func,
                                (GDestroyNotify) rbclt_callback_func_destroy);
    }
  else
    clutter_model_set_filter (model, NULL, NULL, NULL);

  return self;
}

static VALUE
rbclt_model_filter_row (VALUE self, VALUE row)
{
  return clutter_model_filter_row (CLUTTER_MODEL (RVAL2GOBJ (self)),
                                   NUM2UINT (row))
    ? Qtrue : Qfalse;
}

static VALUE
rbclt_model_filter_iter (VALUE self, VALUE iter)
{
  return clutter_model_filter_iter (CLUTTER_MODEL (RVAL2GOBJ (self)),
                                    CLUTTER_MODEL_ITER (RVAL2GOBJ (iter)))
    ? Qtrue : Qfalse;
}

static VALUE
rbclt_model_get_iter_at_row (VALUE self, VALUE row)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));

  return GOBJ2RVALU (clutter_model_get_iter_at_row (model, NUM2UINT (row)));
}

static VALUE
rbclt_model_iter_do_get (VALUE arg)
{
  GetData *data = (GetData *) arg;

  clutter_model_iter_get_value (data->iter, data->column, &data->value);

  data->rvalue = GVAL2RVAL (&data->value);

  return Qnil;
}

static VALUE
rbclt_model_iter_get (int argc, VALUE *argv, VALUE self)
{
  ClutterModel *model;
  int state = 0;
  VALUE ret = Qnil;
  GetData data;

  data.iter = CLUTTER_MODEL_ITER (RVAL2GOBJ (self));
  model = clutter_model_iter_get_model (data.iter);

  if (argc == 1)
    {
      data.column = NUM2UINT (argv[0]);

      memset (&data.value, 0, sizeof (data.value));

      rb_protect (rbclt_model_iter_do_get, (VALUE) &data, &state);
      g_value_unset (&data.value);

      if (state == 0)
        ret = data.rvalue;
    }
  else
    {
      int i;

      ret = rb_ary_new ();

      for (i = 0; i < argc; i++)
        {
          data.column = NUM2UINT (argv[i]);

          memset (&data.value, 0, sizeof (data.value));

          rb_protect (rbclt_model_iter_do_get, (VALUE) &data, &state);
          g_value_unset (&data.value);

          if (state)
            break;

          rb_ary_push (ret, data.rvalue);
        }
    }

  if (state)
    rb_jump_tag (state);

  return ret;
}

static VALUE
rbclt_model_iter_do_set (VALUE arg)
{
  GetData *data = (GetData *) arg;

  rbgobj_rvalue_to_gvalue (data->rvalue, &data->value);
  clutter_model_iter_set_value (data->iter, data->column, &data->value);

  return Qnil;
}

static VALUE
rbclt_model_iter_set (int argc, VALUE *argv, VALUE self)
{
  ClutterModel *model;
  int state = 0;
  GetData data;
  int i;

  if ((argc & 1))
    rb_raise (rb_eArgError, "wrong number of arguments "
              "(paired arguments required)");

  data.iter = CLUTTER_MODEL_ITER (RVAL2GOBJ (self));
  model = clutter_model_iter_get_model (data.iter);

  for (i = 0; i < argc && state == 0; i += 2)
    {
      data.column = NUM2UINT (argv[i]);
      data.rvalue = argv[i + 1];

      memset (&data.value, 0, sizeof (data.value));

      g_value_init (&data.value, clutter_model_get_column_type (model,
                                                                data.column));
      rb_protect (rbclt_model_iter_do_set, (VALUE) &data, &state);
      g_value_unset (&data.value);
    }

  if (state)
    rb_jump_tag (state);

  return self;
}

static VALUE
rbclt_model_iter_array_set (VALUE self, VALUE column, VALUE val)
{
  VALUE argv[2] = { column, val };

  rbclt_model_iter_set (2, argv, self);

  return val;
}

static VALUE
rbclt_model_iter_is_first (VALUE self)
{
  ClutterModelIter *iter = CLUTTER_MODEL_ITER (RVAL2GOBJ (self));

  return clutter_model_iter_is_first (iter) ? Qtrue : Qfalse;
}

static VALUE
rbclt_model_iter_is_last (VALUE self)
{
  ClutterModelIter *iter = CLUTTER_MODEL_ITER (RVAL2GOBJ (self));

  return clutter_model_iter_is_last (iter) ? Qtrue : Qfalse;
}

static VALUE
rbclt_model_iter_next_bang (VALUE self)
{
  ClutterModelIter *iter = CLUTTER_MODEL_ITER (RVAL2GOBJ (self));

  clutter_model_iter_next (iter);

  return self;
}

static VALUE
rbclt_model_iter_next (VALUE self)
{
  ClutterModelIter *iter = CLUTTER_MODEL_ITER (RVAL2GOBJ (self));
  ClutterModelIter *iter_copy = clutter_model_iter_copy (iter);

  return rbclt_model_iter_next_bang (GOBJ2RVALU (iter_copy));
}

static VALUE
rbclt_model_iter_prev_bang (VALUE self)
{
  ClutterModelIter *iter = CLUTTER_MODEL_ITER (RVAL2GOBJ (self));

  clutter_model_iter_prev (iter);

  return self;
}

static VALUE
rbclt_model_iter_prev (VALUE self)
{
  ClutterModelIter *iter = CLUTTER_MODEL_ITER (RVAL2GOBJ (self));
  ClutterModelIter *iter_copy = clutter_model_iter_copy (iter);

  return rbclt_model_iter_prev_bang (GOBJ2RVALU (iter_copy));
}

static VALUE
rbclt_model_iter_dup (VALUE self)
{
  ClutterModelIter *iter = CLUTTER_MODEL_ITER (RVAL2GOBJ (self));

  return GOBJ2RVALU (clutter_model_iter_copy (iter));
}

static VALUE
rbclt_model_each (VALUE self)
{
  VALUE iter = rbclt_model_get_first_iter (self);

  if (!NIL_P (iter))
    {
      while (!RTEST (rbclt_model_iter_is_last (iter)))
        {
          if (RTEST (rbclt_model_filter_iter (self, iter)))
            rb_yield (iter);
          rbclt_model_iter_next_bang (iter);
        }
    }

  return self;
}

static VALUE
rbclt_model_resort (VALUE self)
{
  ClutterModel *model = CLUTTER_MODEL (RVAL2GOBJ (self));

  clutter_model_resort (model);

  return self;
}

void
rbclt_model_init ()
{
  VALUE klass = G_DEF_INTERFACE (CLUTTER_TYPE_MODEL, "Model",
                                 rbclt_c_clutter);

  rb_define_method (klass, "set_types", rbclt_model_set_types, -1);
  rb_define_method (klass, "set_names", rbclt_model_set_names, -1);
  rb_define_method (klass, "append", rbclt_model_append, -1);
  rb_define_method (klass, "prepend", rbclt_model_prepend, -1);
  rb_define_method (klass, "insert", rbclt_model_insert, -1);
  rb_define_method (klass, "insert_value", rbclt_model_insert_value, -1);
  rb_define_method (klass, "remove", rbclt_model_remove, 1);
  rb_define_method (klass, "n_rows", rbclt_model_get_n_rows, 0);
  rb_define_method (klass, "n_columns", rbclt_model_get_n_columns, 0);
  rb_define_method (klass, "get_column_name", rbclt_model_get_column_name, 1);
  rb_define_method (klass, "get_column_type", rbclt_model_get_column_type, 1);
  rb_define_method (klass, "first_iter", rbclt_model_get_first_iter, 0);
  rb_define_method (klass, "last_iter", rbclt_model_get_last_iter, 0);
  rb_define_method (klass, "get_iter_at_row", rbclt_model_get_iter_at_row, 1);
  rb_define_method (klass, "each", rbclt_model_each, 0);
  rb_define_method (klass, "sorting_column",
                    rbclt_model_get_sorting_column, 0);
  rb_define_method (klass, "set_sorting_column",
                    rbclt_model_set_sorting_column, 1);
  rb_define_method (klass, "set_sort", rbclt_model_set_sort, 1);
  rb_define_method (klass, "set_filter", rbclt_model_set_filter, 0);
  rb_define_method (klass, "resort", rbclt_model_resort, 0);
  rb_define_method (klass, "filter_row?", rbclt_model_filter_row, 1);
  rb_define_method (klass, "filter_iter?", rbclt_model_filter_iter, 1);

  G_DEF_SETTERS (klass);

  klass = G_DEF_INTERFACE (CLUTTER_TYPE_MODEL_ITER, "ModelIter",
                           rbclt_c_clutter);
  rb_define_method (klass, "get", rbclt_model_iter_get, -1);
  rb_define_alias (klass, "[]", "get");
  rb_define_method (klass, "set", rbclt_model_iter_set, -1);
  rb_define_method (klass, "[]=", rbclt_model_iter_array_set, 2);
  rb_define_method (klass, "first?", rbclt_model_iter_is_first, 0);
  rb_define_method (klass, "last?", rbclt_model_iter_is_last, 0);
  rb_define_method (klass, "next!", rbclt_model_iter_next_bang, 0);
  rb_define_method (klass, "next", rbclt_model_iter_next, 0);
  rb_define_method (klass, "prev!", rbclt_model_iter_prev_bang, 0);
  rb_define_method (klass, "prev", rbclt_model_iter_prev, 0);
  rb_define_method (klass, "dup", rbclt_model_iter_dup, 0);
}
