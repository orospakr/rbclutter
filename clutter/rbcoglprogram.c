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

static VALUE rb_c_cogl_program;

static void
rb_cogl_program_free (void *ptr)
{
  if (ptr)
    cogl_program_unref (ptr);
}

static VALUE
rb_cogl_program_allocate (VALUE klass)
{
  return Data_Wrap_Struct (klass, NULL, rb_cogl_program_free, NULL);
}

static VALUE
rb_cogl_program_initialize (VALUE self)
{
  CoglHandle program;

  program = cogl_create_program ();

  DATA_PTR (self) = program;

  return Qnil;
}

static CoglHandle
rb_cogl_program_get_handle (VALUE obj)
{
  void *ptr;

  if (!RTEST (rb_obj_is_kind_of (obj, rb_c_cogl_program)))
    rb_raise (rb_eTypeError, "not a Cogl program");

  Data_Get_Struct (obj, void, ptr);

  return (CoglHandle) ptr;
}

static VALUE
rb_cogl_program_attach_shader (VALUE self, VALUE shader_arg)
{
  CoglHandle program = rb_cogl_program_get_handle (self);
  CoglHandle shader = rb_cogl_shader_get_handle (shader_arg);

  cogl_program_attach_shader (program, shader);

  return self;
}

static VALUE
rb_cogl_program_link (VALUE self)
{
  cogl_program_link (rb_cogl_program_get_handle (self));

  return self;
}

static VALUE
rb_cogl_program_use (VALUE self, VALUE program)
{
  if (NIL_P (program))
    cogl_program_use (COGL_INVALID_HANDLE);
  else
    cogl_program_use (rb_cogl_program_get_handle (program));

  return program;
}

static VALUE
rb_cogl_program_get_uniform_location (VALUE self, VALUE uniform_name)
{
  CoglHandle program = rb_cogl_program_get_handle (self);
  COGLint location;

  location = cogl_program_get_uniform_location (program,
						StringValuePtr (uniform_name));

  return INT2NUM (location);
}

static VALUE
rb_cogl_program_uniform (VALUE self, VALUE uniform_no, VALUE value)
{
  cogl_program_uniform_1f (NUM2INT (uniform_no), NUM2DBL (value));

  return self;
}

void
rb_cogl_program_init ()
{
  VALUE klass = rb_define_class_under (rbclt_c_cogl, "Program",
				       rb_cObject);
  rb_c_cogl_program = klass;

  rb_define_alloc_func (klass, rb_cogl_program_allocate);

  rb_define_method (klass, "initialize", rb_cogl_program_initialize, 0);
  rb_define_method (klass, "attach_shader", rb_cogl_program_attach_shader, 1);
  rb_define_alias (klass, "<<", "attach_shader");
  rb_define_method (klass, "link", rb_cogl_program_link, 0);
  rb_define_singleton_method (klass, "use", rb_cogl_program_use, 1);
  rb_define_method (klass, "get_uniform_location",
		    rb_cogl_program_get_uniform_location, 1);
  rb_define_singleton_method (klass, "uniform",
			      rb_cogl_program_uniform, 2);
}
