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

typedef struct _InitializeData InitializeData;

struct _InitializeData
{
  int argc;
  VALUE *argv;
  VALUE self;
  GType *types;
  const gchar **names;
};

static VALUE
rbclt_list_model_do_initialize (VALUE arg)
{
  InitializeData *data = (InitializeData *) arg;
  int i;

  for (i = 0; i < data->argc / 2; i++)
    {
      data->types[i] = CLASS2GTYPE (data->argv[i * 2]);
      data->names[i] = StringValuePtr (data->argv[i * 2 + 1]);
    }

  G_INITIALIZE (data->self, clutter_list_model_newv (data->argc / 2,
                                                     data->types, data->names));

  return Qnil;
}

static VALUE
rbclt_list_model_free_init_data (VALUE arg)
{
  InitializeData *data = (InitializeData *) arg;

  free (data->types);
  free (data->names);

  return Qnil;
}

static VALUE
rbclt_list_model_initialize (int argc, VALUE *argv, VALUE self)
{
  InitializeData data;

  if (argc < 1)
    rb_raise (rb_eArgError, "wrong number of arguments "
              "(at least two required)");
  else if ((argc & 1))
    rb_raise (rb_eArgError, "wrong number of arguments "
              "(paired arguments required)");

  data.argc = argc;
  data.argv = argv;
  data.self = self;
  data.types = ALLOC_N (GType, argc / 2);
  data.names = ALLOC_N (const gchar *, argc/ 2);

  return rb_ensure (rbclt_list_model_do_initialize, (VALUE) &data,
                    rbclt_list_model_free_init_data, (VALUE) &data);
}

void
rbclt_list_model_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_LIST_MODEL, "ListModel",
                             rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_list_model_initialize, -1);
}
