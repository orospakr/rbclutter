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

static VALUE
rbclt_animation_initialize (VALUE self)
{
  ClutterAnimation *animation;

  animation = clutter_animation_new ();
  rbclt_initialize_unowned (self, animation);

  return Qnil;
}

/* static VALUE */
/* rbclt_animation_start (VALUE self) */
/* { */
/*   ClutterAnimation *animation = CLUTTER_ANIMATION (RVAL2GOBJ (self)); */

/*   clutter_animation_start (animation); */

/*   return Qnil; */
/* } */

void
rbclt_animation_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_ANIMATION, "Animation", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_animation_initialize, 0);
  /* rb_define_method (klass, "start", rbclt_animation_start, 0); */
}
