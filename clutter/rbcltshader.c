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

static VALUE
rbclt_shader_initialize (VALUE self)
{
  ClutterShader *shader;

  shader = clutter_shader_new ();

  G_INITIALIZE (self, shader);

  return Qnil;
}

static VALUE
rbclt_shader_compile (VALUE self)
{
  ClutterShader *shader = CLUTTER_SHADER (RVAL2GOBJ (self));
  GError *error = NULL;

  if (!clutter_shader_compile (shader, &error))
    RAISE_GERROR (error);

  return self;
}

static VALUE
rbclt_shader_release (VALUE self)
{
  ClutterShader *shader = CLUTTER_SHADER (RVAL2GOBJ (self));
  clutter_shader_release (shader);
  return self;
}

static VALUE
rbclt_shader_set_uniform (VALUE self, VALUE name, VALUE value)
{
  ClutterShader *shader = CLUTTER_SHADER (RVAL2GOBJ (self));
  /* This could potentially overload to call other variants of
     set_uniform if there were any */
  clutter_shader_set_uniform_1f (shader, StringValuePtr (name),
                                 NUM2DBL (value));
  return self;
}

void
rbclt_shader_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_SHADER, "Shader", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_shader_initialize, 0);
  rb_define_method (klass, "compile", rbclt_shader_compile, 0);
  rb_define_method (klass, "release", rbclt_shader_release, 0);
  rb_define_method (klass, "set_uniform", rbclt_shader_set_uniform, 2);

  G_DEF_ERROR (CLUTTER_SHADER_ERROR, "ShaderError", rbclt_c_clutter,
               rb_eRuntimeError, CLUTTER_TYPE_SHADER_ERROR);

  G_DEF_SETTERS (klass);
}
