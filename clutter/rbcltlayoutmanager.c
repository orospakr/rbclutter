/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2007-2008  Neil Roberts
 * Copyright (C) 2011       Andrew Clunis
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

/* http://docs.clutter-project.org/docs/clutter/stable/ClutterLayoutManager.html */

static VALUE
rbclt_layout_manager_get_preferred_width (VALUE self, VALUE container, VALUE for_height)
{
  ClutterLayoutManager *manager = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (self));
  ClutterContainer *g_container = CLUTTER_CONTAINER (RVAL2GOBJ (container));
  gfloat min_width, nat_width;
  VALUE answer = rb_hash_new ();

  clutter_layout_manager_get_preferred_width (manager, g_container, NUM2INT (for_height), &min_width, &nat_width);
  rb_hash_aset (answer, rb_intern("min"), rb_float_new (min_width));
  rb_hash_aset (answer, rb_intern("nat"), rb_float_new (nat_width));
  
  return answer;
}

static VALUE
rbclt_layout_manager_get_preferred_height (VALUE self, VALUE container, VALUE for_width)
{
  ClutterLayoutManager *manager = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (self));
  ClutterContainer *g_container = CLUTTER_CONTAINER (RVAL2GOBJ (container));
  gfloat min_height, nat_height;
  VALUE answer = rb_hash_new ();

  clutter_layout_manager_get_preferred_height (manager, g_container, NUM2INT (for_width), &min_height, &nat_height);
  rb_hash_aset (answer, rb_intern("min"), rb_float_new (min_height));
  rb_hash_aset (answer, rb_intern("nat"), rb_float_new (nat_height));

  return answer;
}

static VALUE
rbclt_layout_manager_allocate (VALUE self, VALUE container, VALUE actorbox, VALUE flags)
{
  ClutterLayoutManager *manager = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (self));
  ClutterContainer *g_container = CLUTTER_CONTAINER (RVAL2GOBJ (container));
  ClutterActorBox g_actorbox = *(ClutterActorBox *) RVAL2BOXED (actorbox, CLUTTER_TYPE_ACTOR_BOX);
  ClutterAllocationFlags g_flags = RVAL2GENUM (flags, CLUTTER_TYPE_ALLOCATION_FLAGS);

  clutter_layout_manager_allocate (manager, g_container, &g_actorbox, g_flags);

  return Qnil;
}

static VALUE
rbclt_layout_manager_layout_changed (VALUE self)
{
  ClutterLayoutManager *manager = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (self));

  clutter_layout_manager_layout_changed (manager);
  return Qnil;
}

static VALUE
rbclt_layout_manager_set_container (VALUE self, VALUE container)
{
  ClutterLayoutManager *manager = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (self));
  ClutterContainer *g_container = CLUTTER_CONTAINER (RVAL2GOBJ (container));
  
  clutter_layout_manager_set_container (manager, g_container);
  return Qnil;
}

static VALUE
rbclt_layout_manager_get_child_meta (VALUE self, VALUE container, VALUE actor)
{
  ClutterLayoutManager *manager = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (self));
  ClutterContainer *g_container = CLUTTER_CONTAINER (RVAL2GOBJ (container));
  ClutterActor *g_actor = CLUTTER_ACTOR (RVAL2GOBJ (actor));
  ClutterLayoutMeta *layout_meta = CLUTTER_LAYOUT_META (clutter_layout_manager_get_child_meta (manager, g_container, g_actor));

  return GOBJ2RVAL (layout_meta);
}

/* I'm skipping child_set(), because it is another unbindable varargs
   routine.  Use child_set_property() instead.  I could wrap it in
   code for iterating through a hash, but I'm lazy */

static VALUE
rbclt_layout_manager_child_set_property (VALUE self, VALUE container, VALUE actor, VALUE property, VALUE value)
{
  ClutterLayoutManager *manager = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (self));
  ClutterContainer *g_container = CLUTTER_CONTAINER (RVAL2GOBJ (container));
  ClutterActor *g_actor = CLUTTER_ACTOR (RVAL2GOBJ (actor));
  GValue g_value;
  g_value_init (&g_value, RVAL2GTYPE (value));

  rbgobj_rvalue_to_gvalue (value, &g_value);
  clutter_layout_manager_child_set_property (manager, g_container, g_actor, RVAL2CSTR (property), &g_value);
  g_value_unset (&g_value);
  return Qnil;
}

static VALUE
rbclt_layout_manager_child_get_property (VALUE self, VALUE container, VALUE actor, VALUE property)
{
  ClutterLayoutManager *manager = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (self));
  ClutterContainer *g_container = CLUTTER_CONTAINER (RVAL2GOBJ (container));
  ClutterActor *g_actor = CLUTTER_ACTOR (RVAL2GOBJ (actor));

  GValue answer;

  clutter_layout_manager_child_get_property (manager, g_container, g_actor, RVAL2CSTR (property), &answer);
  VALUE r_answer = GVAL2RVAL (&answer);

  g_value_unset (&answer);
  
  return r_answer;
}

static VALUE
rbclt_layout_manager_begin_animation (VALUE self, VALUE duration, VALUE mode)
{
  ClutterLayoutManager *manager = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_layout_manager_begin_animation (manager, NUM2UINT (duration), RVAL2GENUM (mode, CLUTTER_TYPE_ANIMATION_MODE)));
}

static VALUE
rbclt_layout_manager_end_animation (VALUE self)
{
  ClutterLayoutManager *manager = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (self));
  clutter_layout_manager_end_animation (manager);
  return Qnil;
}

static VALUE
rbclt_layout_manager_get_animation_progress (VALUE self)
{
  ClutterLayoutManager *manager = CLUTTER_LAYOUT_MANAGER (RVAL2GOBJ (self));
  return DBL2NUM (clutter_layout_manager_get_animation_progress (manager));
}

void
rbclt_layout_manager_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_LAYOUT_MANAGER, "LayoutManager", rbclt_c_clutter);

  rb_define_method (klass, "get_preferred_width", rbclt_layout_manager_get_preferred_width, 2);
  rb_define_method (klass, "get_preferred_height", rbclt_layout_manager_get_preferred_height, 2);
  rb_define_method (klass, "allocate", rbclt_layout_manager_allocate, 3);
  rb_define_method (klass, "layout_changed", rbclt_layout_manager_layout_changed, 0);
  rb_define_method (klass, "set_container", rbclt_layout_manager_set_container, 1);
  rb_define_method (klass, "get_child_meta", rbclt_layout_manager_get_child_meta, 2);
  rb_define_method (klass, "child_set_property", rbclt_layout_manager_child_set_property, 4);
  rb_define_method (klass, "child_get_property", rbclt_layout_manager_child_get_property, 3);
  rb_define_method (klass, "begin_animation", rbclt_layout_manager_begin_animation, 2);
  rb_define_method (klass, "end_animation", rbclt_layout_manager_end_animation, 0);
  rb_define_method (klass, "get_animation_progress", rbclt_layout_manager_get_animation_progress, 0);
}
