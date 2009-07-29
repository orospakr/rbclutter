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
#include "rbcltstagemanager.h"

static VALUE
rbclt_stage_manager_get_default ()
{
  ClutterStageManager *stage_manager = clutter_stage_manager_get_default ();

  if (stage_manager == NULL)
    return Qnil;
  else
    return GOBJ2RVAL (stage_manager);
}

static VALUE
rbclt_stage_manager_get_stages (VALUE self)
{
  ClutterStageManager *stage_manager = CLUTTER_STAGE_MANAGER (RVAL2GOBJ (self));

  return GSLIST2ARYF (clutter_stage_manager_list_stages (stage_manager));
}

static void
rbclt_stage_manager_mark_callback (gpointer stage, gpointer user_data)
{
  rbgobj_gc_mark_instance (stage);
}

static void
rbclt_stage_manager_mark (void *p)
{
  GSList *stage_list;

  /* Mark all of the stages as well */
  stage_list = clutter_stage_manager_list_stages (CLUTTER_STAGE_MANAGER (p));
  g_slist_foreach (stage_list, rbclt_stage_manager_mark_callback, NULL);
  g_slist_free (stage_list);
}

void
rbclt_stage_manager_init_global_mark ()
{
  static VALUE default_stage_manager = Qnil;

  /* Store the default stage manager in a global variable so that we
     can mark all of the stages when the garbage collector is run */
  if (default_stage_manager == Qnil)
    {
      default_stage_manager = rbclt_stage_manager_get_default ();
      rb_gc_register_address (&default_stage_manager);
    }
}

void
rbclt_stage_manager_init ()
{
  VALUE klass = G_DEF_CLASS2 (CLUTTER_TYPE_STAGE_MANAGER, "StageManager",
                              rbclt_c_clutter,
                              rbclt_stage_manager_mark, NULL);

  rb_define_singleton_method (klass, "get_default",
                              rbclt_stage_manager_get_default, 0);

  rb_define_method (klass, "stages", rbclt_stage_manager_get_stages, 0);
}
