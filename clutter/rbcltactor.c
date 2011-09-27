/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2007-2008  Neil Roberts
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

VALUE rbclt_c_actor;

static VALUE
rbclt_actor_subclass_initialize (int argc, VALUE *argv, VALUE self)
{
  GType gtype = RVAL2GTYPE (self);
  ClutterActor *actor;

  if (gtype == CLUTTER_TYPE_ACTOR)
    rb_raise (rb_eArgError, "Clutter::Actor can not be instantiated directly");

  /* Chain up to let GLib::Object.new create the right instance */
  rb_call_super (argc, argv);

  /* Claim the floating reference if it hasn't been claimed
     already. Doing it conditionally hopefully protects against the
     GObject bindings being fixed one day so that they understand
     GInitiallyUnowned */
  actor = RVAL2GOBJ (self);
  if (g_object_is_floating (actor))
    g_object_ref_sink (actor);

  return Qnil;
}

static VALUE
rbclt_actor_type_register (int argc, VALUE *argv, VALUE self)
{
  VALUE result;
  VALUE mod;

  /* GObject::type_register will define and include a stub module to
     override the initialize constructor with one that calls
     gobj_initialize. That function isn't good enough for ClutterActor
     because it doesn't understand GInitiallyUnowned so it doesn't
     claim the floating reference. So when the actor is added to a
     container the container will steal Ruby's reference. To work
     around this we include another stub module to redefine the
     initialize method again */

  /* Let the real type_register function run */
  result = rb_call_super (argc, argv);

  /* Define a stub module specifically for this new subclass */
  mod = rb_define_module_under (self, "ClutterGObjectHook__");
  /* Give it a constructor */
  rb_define_method (mod, "initialize",
                    rbclt_actor_subclass_initialize, -1);
  /* Make the new class include the module */
  rb_include_module (self, mod);

  return result;
 }

static VALUE
rbclt_actor_show (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_show (actor);
  return self;
}

static VALUE
rbclt_actor_show_all (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_show_all (actor);
  return self;
}

static VALUE
rbclt_actor_hide (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_hide (actor);
  return self;
}

static VALUE
rbclt_actor_hide_all (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_hide_all (actor);
  return self;
}

static VALUE
rbclt_actor_realize (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_realize (actor);
  return self;
}

static VALUE
rbclt_actor_unrealize (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_unrealize (actor);
  return self;
}

static VALUE
rbclt_actor_paint (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_paint (actor);
  return self;
}

static VALUE
rbclt_actor_pick (VALUE self, VALUE color)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_pick (actor, ((ClutterColor *) RVAL2BOXED (self, CLUTTER_TYPE_COLOR)));
  return self;
}

static VALUE
rbclt_actor_queue_redraw (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_queue_redraw (actor);
  return self;
}

static VALUE
rbclt_actor_queue_relayout (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_queue_relayout (actor);
  return self;
}

static VALUE
rbclt_actor_destroy (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_destroy (actor);
  return self;
}

static VALUE
rbclt_actor_get_preferred_width (int argc, VALUE *argv, VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  VALUE for_height;
  gfloat min_width, natural_width;

  rb_scan_args (argc, argv, "01", &for_height);

  clutter_actor_get_preferred_width (actor,
                                     NIL_P (for_height)
                                     ? -1 : NUM2INT (for_height),
                                     &min_width, &natural_width);
  return rb_ary_new3 (2,
                      rb_float_new (min_width),
                      rb_float_new (natural_width));
}

static VALUE
rbclt_actor_get_preferred_height (int argc, VALUE *argv, VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  VALUE for_width;
  gfloat min_height, natural_height;

  rb_scan_args (argc, argv, "01", &for_width);

  clutter_actor_get_preferred_height (actor,
                                      NIL_P (for_width)
                                      ? -1 : NUM2INT (for_width),
                                      &min_height, &natural_height);
  return rb_ary_new3 (2,
                      rb_float_new (min_height),
                      rb_float_new (natural_height));
}

static VALUE
rbclt_actor_get_preferred_size (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  gfloat min_width, min_height, natural_width, natural_height;

  clutter_actor_get_preferred_size (actor, &min_width, &min_height,
                                    &natural_width, &natural_height);

  return rb_ary_new3 (4, rb_float_new (min_width),
                      rb_float_new (min_height),
                      rb_float_new (natural_width),
                      rb_float_new (natural_height));
}

static VALUE
rbclt_actor_allocate (VALUE self, VALUE box_arg, VALUE origin_changed)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterActorBox box
    = *(ClutterActorBox *) RVAL2BOXED (box_arg, CLUTTER_TYPE_ACTOR_BOX);

  clutter_actor_allocate (actor, &box, RTEST (origin_changed));

  return self;
}

static VALUE
rbclt_actor_allocate_preferred_size (VALUE self, VALUE origin_changed)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));

  clutter_actor_allocate_preferred_size (actor, RTEST (origin_changed));

  return self;
}

static VALUE
rbclt_actor_get_allocation_coords (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  gint x_1, y_1, x_2, y_2;

  clutter_actor_get_allocation_coords (actor, &x_1, &y_1, &x_2, &y_2);

  return rb_ary_new3 (4, INT2NUM (x_1), INT2NUM (y_1),
                      INT2NUM (x_2), INT2NUM (y_2));
}

static VALUE
rbclt_actor_get_allocation_box (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterActorBox box;

  clutter_actor_get_allocation_box (actor, &box);

  return BOXED2RVAL (&box, CLUTTER_TYPE_ACTOR_BOX);
}

static VALUE
rbclt_actor_get_allocation_geometry (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterGeometry geometry;

  clutter_actor_get_allocation_geometry (actor, &geometry);

  return BOXED2RVAL (&geometry, CLUTTER_TYPE_GEOMETRY);
}

static VALUE
rbclt_actor_get_allocation_vertices (int argc, VALUE *argv, VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterActor *ancestor;
  ClutterVertex verts[4];
  VALUE ancestor_arg;
  VALUE values[4];
  int i;

  rb_scan_args (argc, argv, "01", &ancestor_arg);
  ancestor = NIL_P (ancestor_arg)
    ? NULL : CLUTTER_ACTOR (RVAL2GOBJ (ancestor_arg));

  clutter_actor_get_allocation_vertices (actor, ancestor, verts);

  for (i = 0; i < 4; i++)
    values[i] = BOXED2RVAL (verts + i, CLUTTER_TYPE_VERTEX);

  return rb_ary_new4 (4, values);
}

static VALUE
rbclt_actor_get_abs_allocation_vertices (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterVertex verts[4];
  VALUE values[4];
  int i;

  clutter_actor_get_abs_allocation_vertices (actor, verts);

  for (i = 0; i < 4; i++)
    values[i] = BOXED2RVAL (verts + i, CLUTTER_TYPE_VERTEX);

  return rb_ary_new4 (4, values);
}

static VALUE
rbclt_actor_add_constraint (VALUE self, VALUE constraint)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterConstraint *g_constraint = CLUTTER_CONSTRAINT (RVAL2GOBJ (constraint));

  clutter_actor_add_constraint (actor, g_constraint);

  return Qnil;
}

static VALUE
rbclt_actor_add_constraint_with_name (VALUE self, VALUE name, VALUE constraint)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterConstraint *g_constraint = CLUTTER_CONSTRAINT (RVAL2GOBJ (constraint));

  clutter_actor_add_constraint_with_name (actor, RVAL2CSTR(name), g_constraint);
  return Qnil;
}

static VALUE
rbclt_actor_geometry (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterGeometry geometry;
  clutter_actor_get_geometry (actor, &geometry);
  return BOXED2RVAL (&geometry, CLUTTER_TYPE_GEOMETRY);
}

static VALUE
rbclt_actor_set_geometry (VALUE self, VALUE geometry_arg)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterGeometry *geometry = (ClutterGeometry *) RVAL2BOXED (geometry_arg, CLUTTER_TYPE_GEOMETRY);
  clutter_actor_set_geometry (actor, geometry);
  return self;
}

static VALUE
rbclt_actor_set_size (VALUE self, VALUE width, VALUE height)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_set_size (actor, NUM2INT (width), NUM2INT (height));
  return self;
}

static VALUE
rbclt_actor_get_transformed_size (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  gfloat width, height;
  clutter_actor_get_transformed_size (actor, &width, &height);
  return rb_ary_new3 (2, rb_float_new (width), rb_float_new (height));
}

static VALUE
rbclt_actor_set_position (VALUE self, VALUE x, VALUE y)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_set_position (actor, NUM2INT (x), NUM2INT (y));
  return self;
}

static VALUE
rbclt_actor_get_transformed_position (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  gfloat x, y;
  clutter_actor_get_transformed_position (actor, &x, &y);
  return rb_ary_new3 (2, rb_float_new (x), rb_float_new (y));
}

static VALUE
rbclt_actor_set_rotation (VALUE self, VALUE axis, VALUE angle,
                          VALUE x, VALUE y, VALUE z)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_set_rotation (actor,
                              RVAL2GENUM (axis, CLUTTER_TYPE_ROTATE_AXIS),
                              NUM2DBL (angle),
                              NUM2INT (x),
                              NUM2INT (y),
                              NUM2INT (z));
  return self;
}

static VALUE
rbclt_actor_get_rotation (VALUE self, VALUE axis)
{
  gdouble angle;
  gfloat x = 0, y = 0, z = 0;
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));

  angle = clutter_actor_get_rotation (actor,
                                      RVAL2GENUM (axis,
                                                  CLUTTER_TYPE_ROTATE_AXIS),
                                      &x, &y, &z);

  return rb_ary_new3 (4, rb_float_new (angle),
                      rb_float_new (x), rb_float_new (y), rb_float_new (z));
}

static VALUE
rbclt_actor_get_paint_opacity (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  return INT2NUM (clutter_actor_get_paint_opacity (actor));
}

static VALUE
rbclt_actor_gid (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  return UINT2NUM (clutter_actor_get_gid (actor));
}

static VALUE
rbclt_actor_remove_clip (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_remove_clip (actor);
  return self;
}

static VALUE
rbclt_actor_set_clip (VALUE self,
                      VALUE xoff,
                      VALUE yoff,
                      VALUE width,
                      VALUE height)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ(self));
  clutter_actor_set_clip (actor,
                          NUM2INT (xoff),
                          NUM2INT (yoff),
                          NUM2INT (width),
                          NUM2INT (height));
}

static VALUE
rbclt_actor_set_parent (VALUE self, VALUE parent_arg)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterActor *parent = CLUTTER_ACTOR (RVAL2GOBJ (parent_arg));
  clutter_actor_set_parent (actor, parent);
  return self;
}

static VALUE
rbclt_actor_parent (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_actor_get_parent (actor));
}

static VALUE
rbclt_actor_reparent (VALUE self, VALUE parent_arg)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterActor *parent = CLUTTER_ACTOR (RVAL2GOBJ (parent_arg));
  clutter_actor_reparent (actor, parent);
  return self;
}

static VALUE
rbclt_actor_unparent (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_unparent (actor);
  return self;
}

static VALUE
rbclt_actor_raise (VALUE self, VALUE below_arg)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterActor *below = CLUTTER_ACTOR (RVAL2GOBJ (below_arg));
  clutter_actor_raise (actor, below);
  return self;
}

static VALUE
rbclt_actor_lower (VALUE self, VALUE above_arg)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterActor *above = CLUTTER_ACTOR (RVAL2GOBJ (above_arg));
  clutter_actor_lower (actor, above);
  return self;
}

static VALUE
rbclt_actor_raise_top (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_raise_top (actor);
  return self;
}

static VALUE
rbclt_actor_lower_bottom (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_lower_bottom (actor);
  return self;
}

static VALUE
rbclt_actor_set_depth (VALUE self, VALUE depth)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_set_depth (actor, NUM2INT (depth));
  return self;
}

static VALUE
rbclt_actor_depth (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  return INT2NUM (clutter_actor_get_depth (actor));
}

static VALUE
rbclt_actor_set_scale (VALUE self, VALUE scale_x, VALUE scale_y)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_set_scale (actor, NUM2DBL (scale_x), NUM2DBL (scale_y));
  return self;
}

static VALUE
rbclt_actor_scale (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  gdouble scale_x, scale_y;
  clutter_actor_get_scale (actor, &scale_x, &scale_y);
  return rb_ary_new3 (2, rb_float_new (scale_x), rb_float_new (scale_y));
}

static VALUE
rbclt_actor_size (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  gfloat width, height;
  clutter_actor_get_size (actor, &width, &height);
  return rb_ary_new3 (2, rb_float_new (width), rb_float_new (height));
}

static VALUE
rbclt_actor_move_by (VALUE self, VALUE dx, VALUE dy)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_move_by (actor, NUM2INT (dx), NUM2INT (dy));
  return self;
}

static VALUE
rbclt_actor_apply_transform_to_point (VALUE self, VALUE point_arg)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterVertex point = *(ClutterVertex *) RVAL2BOXED (point_arg,
                                                       CLUTTER_TYPE_VERTEX);
  ClutterVertex vertex;
  clutter_actor_apply_transform_to_point (actor, &point, &vertex);
  return BOXED2RVAL (&vertex, CLUTTER_TYPE_VERTEX);
}

static VALUE
rbclt_actor_apply_relative_transform_to_point (VALUE self,
                                               VALUE ancestor,
                                               VALUE point_arg)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterVertex point = *(ClutterVertex *) RVAL2BOXED (point_arg,
                                                       CLUTTER_TYPE_VERTEX);
  ClutterVertex vertex;

  clutter_actor_apply_relative_transform_to_point (actor,
                                                   RVAL2GOBJ (ancestor),
                                                   &point, &vertex);

  return BOXED2RVAL (&vertex, CLUTTER_TYPE_VERTEX);
}

static VALUE
rbclt_actor_should_pick_paint (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  return clutter_actor_should_pick_paint (actor) ? Qtrue : Qfalse;
}

static VALUE
rbclt_actor_set_shader (VALUE self, VALUE shader)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_set_shader (actor, CLUTTER_SHADER (RVAL2GOBJ (shader)));
  return self;
}

static VALUE
rbclt_actor_shader (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_actor_get_shader (actor));
}

static VALUE
rbclt_actor_set_shader_param (VALUE self, VALUE param, VALUE value)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_set_shader_param_float (actor, StringValuePtr (param),
                                  NUM2FLOAT (value));
  return self;
}

static VALUE
rbclt_actor_set_anchor_point (VALUE self, VALUE x, VALUE y)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_set_anchor_point (actor, NUM2INT (x), NUM2INT (y));
  return self;
}

static VALUE
rbclt_actor_move_anchor_point (VALUE self, VALUE x, VALUE y)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  clutter_actor_move_anchor_point (actor, NUM2INT (x), NUM2INT (y));
  return self;
}

static VALUE
rbclt_actor_set_anchor_point_from_gravity (VALUE self, VALUE gravity)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterGravity gravity_val = RVAL2GENUM (gravity, CLUTTER_TYPE_GRAVITY);
  clutter_actor_set_anchor_point_from_gravity (actor, gravity_val);
  return self;
}

static VALUE
rbclt_actor_move_anchor_point_from_gravity (VALUE self, VALUE gravity)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterGravity gravity_val = RVAL2GENUM (gravity, CLUTTER_TYPE_GRAVITY);
  clutter_actor_move_anchor_point_from_gravity (actor, gravity_val);
  return self;
}

static VALUE
rbclt_actor_is_rotated (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  return clutter_actor_is_rotated (actor) ? Qtrue : Qfalse;
}

static VALUE
rbclt_actor_is_scaled (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  return clutter_actor_is_scaled (actor) ? Qtrue : Qfalse;
}

static VALUE
rbclt_actor_stage (VALUE self)
{
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  return GOBJ2RVAL (clutter_actor_get_stage (actor));
}

static VALUE
rbclt_actor_animate (VALUE self, VALUE mode, VALUE duration, VALUE properties)
{
  gint c = 0;
  VALUE current_property = Qnil;
  gchar ** properties_vector = NULL;
  GValue * values_vector = NULL;
  ClutterActor *actor = CLUTTER_ACTOR (RVAL2GOBJ (self));
  ClutterAnimation *animation;

  VALUE props_lengthv = rb_funcall (properties, rb_intern("length"), 0);
  gint props_length = NUM2INT (props_lengthv);

  // TODO validate properties is a hash

  // man this is such an annoying way to iterate through a hash...

  properties_vector = g_malloc (sizeof (gchar*) * props_length);
  values_vector = g_malloc (sizeof (GValue) * props_length);

  VALUE keys = rb_funcall (properties, rb_intern("keys"), 0);

  for (c = 0; c < props_length; c++)
    {
      VALUE key = rb_ary_entry (keys, c);
      current_property = rb_hash_aref (properties, key);
      GValue gv = {0, };

      if (current_property == Qnil) {
        rb_raise(rb_eArgError, "Properties must not be nil!");
        return Qnil;
       }

      rbgobj_initialize_gvalue(&gv, current_property);

      properties_vector[c] = StringValuePtr (key);
      values_vector[c] = gv;
      // printf("Hello: %s : %s \n", StringValuePtr (key), StringValuePtr (current_property));
    }

  gint mode_int = NUM2INT (mode);
  gint duration_int = NUM2INT (duration);

  animation = clutter_actor_animatev (actor, mode_int, duration_int, props_length, (const gchar * const *)properties_vector, values_vector);

  g_free(properties_vector);
  g_free(values_vector);

  return GOBJ2RVAL (animation);
}

void
rbclt_actor_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_ACTOR, "Actor", rbclt_c_clutter);

  rbclt_c_actor = klass;

  rb_define_singleton_method (klass, "type_register",
                              rbclt_actor_type_register, -1);

  rb_define_method (klass, "show", rbclt_actor_show, 0);
  rb_define_method (klass, "show_all", rbclt_actor_show_all, 0);
  rb_define_method (klass, "hide", rbclt_actor_hide, 0);
  rb_define_method (klass, "hide_all", rbclt_actor_hide_all, 0);
  rb_define_method (klass, "realize", rbclt_actor_realize, 0);
  rb_define_method (klass, "unrealize", rbclt_actor_unrealize, 0);
  rb_define_method (klass, "paint", rbclt_actor_paint, 0);
  rb_define_method (klass, "pick", rbclt_actor_pick, 1);
  rb_define_method (klass, "queue_redraw", rbclt_actor_queue_redraw, 0);
  rb_define_method (klass, "queue_relayout", rbclt_actor_queue_relayout, 0);
  rb_define_method (klass, "destroy", rbclt_actor_destroy, 0);
  rb_define_method (klass, "get_preferred_width",
                    rbclt_actor_get_preferred_width, -1);
  rb_define_alias (klass, "preferred_width", "get_preferred_width");
  rb_define_method (klass, "get_preferred_height",
                    rbclt_actor_get_preferred_height, -1);
  rb_define_alias (klass, "preferred_height", "get_preferred_height");
  rb_define_method (klass, "preferred_size",
                    rbclt_actor_get_preferred_size, 0);
  rb_define_method (klass, "allocate", rbclt_actor_allocate, 2);
  rb_define_method (klass, "allocate_preferred_size",
                    rbclt_actor_allocate_preferred_size, 1);
  rb_define_method (klass, "allocation_coords",
                    rbclt_actor_get_allocation_coords, 0);
  rb_define_method (klass, "allocation_box",
                    rbclt_actor_get_allocation_box, 0);
  rb_define_method (klass, "allocation_geometry",
                    rbclt_actor_get_allocation_geometry, 0);
  rb_define_method (klass, "get_allocation_vertices",
                    rbclt_actor_get_allocation_vertices, -1);
  rb_define_alias (klass, "allocation_vertices", "get_allocation_vertices");
  rb_define_method (klass, "abs_allocation_vertices",
                    rbclt_actor_get_abs_allocation_vertices, 0);
  rb_define_method (klass, "add_constraint", rbclt_actor_add_constraint, 1);
  rb_define_method (klass, "add_constraint_with_name", rbclt_actor_add_constraint_with_name, 2);
  rb_define_method (klass, "geometry", rbclt_actor_geometry, 0);
  rb_define_method (klass, "set_geometry", rbclt_actor_set_geometry, 1);
  rb_define_method (klass, "set_size", rbclt_actor_set_size, 2);
  rb_define_method (klass, "set_position", rbclt_actor_set_position, 2);
  rb_define_method (klass, "transformed_size",
                    rbclt_actor_get_transformed_size, 0);
  rb_define_method (klass, "transformed_position",
                    rbclt_actor_get_transformed_position, 0);
  rb_define_method (klass, "set_rotation", rbclt_actor_set_rotation, 5);
  rb_define_method (klass, "get_rotation", rbclt_actor_get_rotation, 1);
  rb_define_method (klass, "paint_opacity", rbclt_actor_get_paint_opacity, 0);
  rb_define_method (klass, "gid", rbclt_actor_gid, 0);
  rb_define_method (klass, "remove_clip", rbclt_actor_remove_clip, 0);
  rb_define_method (klass, "set_clip", rbclt_actor_set_clip, 4);
  rb_define_method (klass, "set_parent", rbclt_actor_set_parent, 1);
  rb_define_method (klass, "parent", rbclt_actor_parent, 0);
  rb_define_method (klass, "reparent", rbclt_actor_reparent, 1);
  rb_define_method (klass, "unparent", rbclt_actor_unparent, 0);
  rb_define_method (klass, "raise", rbclt_actor_raise, 1);
  rb_define_method (klass, "lower", rbclt_actor_lower, 1);
  rb_define_method (klass, "raise_top", rbclt_actor_raise_top, 0);
  rb_define_method (klass, "lower_bottom", rbclt_actor_lower_bottom, 0);
  rb_define_method (klass, "set_depth", rbclt_actor_set_depth, 1);
  rb_define_method (klass, "depth", rbclt_actor_depth, 0);
  rb_define_method (klass, "set_scale", rbclt_actor_set_scale, 2);
  rb_define_method (klass, "scale", rbclt_actor_scale, 0);
  rb_define_method (klass, "size", rbclt_actor_size, 0);
  rb_define_method (klass, "move_by", rbclt_actor_move_by, 2);
  rb_define_method (klass, "apply_transform_to_point",
                    rbclt_actor_apply_transform_to_point, 1);
  rb_define_method (klass, "apply_relative_transform_to_point",
                    rbclt_actor_apply_relative_transform_to_point, 2);
  rb_define_method (klass, "should_pick_paint",
                    rbclt_actor_should_pick_paint, 0);
  rb_define_alias (klass, "pick_paint?", "should_pick_paint");
  rb_define_method (klass, "set_shader", rbclt_actor_set_shader, 1);
  rb_define_method (klass, "shader", rbclt_actor_shader, 0);
  rb_define_method (klass, "set_shader_param", rbclt_actor_set_shader_param, 2);
  rb_define_method (klass, "set_anchor_point", rbclt_actor_set_anchor_point, 2);
  rb_define_method (klass, "move_anchor_point",
                    rbclt_actor_move_anchor_point, 2);
  rb_define_method (klass, "set_anchor_point_from_gravity",
                    rbclt_actor_set_anchor_point_from_gravity, 1);
  rb_define_method (klass, "move_anchor_point_from_gravity",
                    rbclt_actor_move_anchor_point_from_gravity, 1);
  rb_define_method (klass, "rotated?", rbclt_actor_is_rotated, 0);
  rb_define_method (klass, "scaled?", rbclt_actor_is_scaled, 0);
  rb_define_method (klass, "stage", rbclt_actor_stage, 0);
  rb_define_method (klass, "animate", rbclt_actor_animate, 3);

  G_DEF_SETTERS (klass);
}
