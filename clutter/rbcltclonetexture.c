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
#include <clutter/clutter-clone-texture.h>

#include "rbclutter.h"

static VALUE
rbclt_clone_texture_initialize (VALUE self, VALUE other)
{
  ClutterActor *actor;

  actor = clutter_clone_texture_new (RVAL2GOBJ (other));

  rbclt_initialize_unowned (self, actor);

  return Qnil;
}

void
rbclt_clone_texture_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_CLONE_TEXTURE, "CloneTexture", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_clone_texture_initialize, 1);
}
