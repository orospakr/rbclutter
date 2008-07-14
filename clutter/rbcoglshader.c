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
#include <cogl/cogl.h>

#include "rbclutter.h"
#include "rbcoglshader.h"

static VALUE rb_c_cogl_shader;

static void
rb_cogl_shader_free (void *ptr)
{
  if (ptr)
    cogl_shader_unref (ptr);
}

static VALUE
rb_cogl_shader_allocate (VALUE klass)
{
  return Data_Wrap_Struct (klass, NULL, rb_cogl_shader_free, NULL);
}

static VALUE
rb_cogl_shader_initialize (VALUE self, VALUE shader_type)
{
  CoglHandle shader;

  shader = cogl_create_shader (NUM2UINT (shader_type));

  DATA_PTR (self) = shader;

  return Qnil;
}

CoglHandle
rb_cogl_shader_get_handle (VALUE obj)
{
  void *ptr;

  if (!RTEST (rb_obj_is_kind_of (obj, rb_c_cogl_shader)))
    rb_raise (rb_eTypeError, "not a Cogl shader");

  Data_Get_Struct (obj, void, ptr);

  return (CoglHandle) ptr;
}

static VALUE
rb_cogl_shader_source (VALUE self, VALUE source)
{
  cogl_shader_source (rb_cogl_shader_get_handle (self),
		      StringValuePtr (source));

  return self;
}

static VALUE
rb_cogl_shader_compile (VALUE self)
{
  cogl_shader_compile (rb_cogl_shader_get_handle (self));

  return self;
}

static VALUE
rb_cogl_shader_get_info_log (VALUE self)
{
  CoglHandle shader = rb_cogl_shader_get_handle (self);
  char buf[512];
  
  buf[0] = '\0';
  buf[sizeof (buf) - 1] = '\0';
  cogl_shader_get_info_log (shader, sizeof (buf) - 1, buf);

  return rb_str_new2 (buf);
}

static VALUE
rb_cogl_shader_get_parameter (VALUE self, VALUE pname)
{
  CoglHandle shader = rb_cogl_shader_get_handle (self);
  COGLint value;

  cogl_shader_get_parameteriv (shader, NUM2UINT (pname), &value);

  return INT2NUM (value);
}

void
rb_cogl_shader_init ()
{
  VALUE klass = rb_define_class_under (rbclt_c_cogl, "Shader",
				       rb_cObject);
  rb_c_cogl_shader = klass;

  rb_define_alloc_func (klass, rb_cogl_shader_allocate);

  rb_define_method (klass, "initialize", rb_cogl_shader_initialize, 1);
  rb_define_method (klass, "source", rb_cogl_shader_source, 1);
  rb_define_method (klass, "compile", rb_cogl_shader_compile, 0);
  rb_define_method (klass, "info_log", rb_cogl_shader_get_info_log, 0);
  rb_define_method (klass, "get_parameter", rb_cogl_shader_get_parameter, 1);
}
