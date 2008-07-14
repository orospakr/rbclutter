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
#include "rbcogltexture.h"

static VALUE rb_c_cogl_offscreen;
static VALUE rb_c_cogl_offscreen_error;

static void
rb_cogl_offscreen_free (void *ptr)
{
  if (ptr)
    cogl_offscreen_unref (ptr);
}

static VALUE
rb_cogl_offscreen_allocate (VALUE klass)
{
  return Data_Wrap_Struct (klass, NULL, rb_cogl_offscreen_free, NULL);
}

static VALUE
rb_cogl_offscreen_initialize (int argc, VALUE *argv, VALUE self)
{
  VALUE tex;
  CoglHandle offscreen;

  rb_scan_args (argc, argv, "01", &tex);

  if (NIL_P (tex))
    offscreen = cogl_offscreen_new_multisample ();
  else
    {
      CoglHandle tex_handle = rb_cogl_texture_get_handle (tex);
      offscreen = cogl_offscreen_new_to_texture (tex_handle);
    }

  if (offscreen == COGL_INVALID_HANDLE)
    rb_raise (rb_c_cogl_offscreen_error, "failed to create offscreen object");

  DATA_PTR (self) = offscreen;

  return Qnil;
}

static CoglHandle
rb_cogl_offscreen_get_handle (VALUE obj)
{
  void *ptr;

  if (!RTEST (rb_obj_is_kind_of (obj, rb_c_cogl_offscreen)))
    rb_raise (rb_eTypeError, "not a Cogl offscreen");

  Data_Get_Struct (obj, void, ptr);

  return (CoglHandle) ptr;
}

static VALUE
rb_cogl_offscreen_blit (int argc, VALUE *argv, VALUE self)
{
  if (argc == 1)
    cogl_offscreen_blit (rb_cogl_offscreen_get_handle (self),
			 rb_cogl_offscreen_get_handle (argv[0]));
  else
    {
      VALUE dst_buffer, src_x, src_y, src_w, src_h, dst_x, dst_y, dst_w, dst_h;

      rb_scan_args (argc, argv, "72",
		    &dst_buffer, &src_x, &src_y, &src_w, &src_h,
		    &dst_x, &dst_y, &dst_w, &dst_h);

      cogl_offscreen_blit_region (rb_cogl_offscreen_get_handle (self),
				  rb_cogl_offscreen_get_handle (dst_buffer),
				  NUM2INT (src_x), NUM2INT (src_y),
				  NUM2INT (src_w), NUM2INT (src_h),
				  NUM2INT (dst_x), NUM2INT (dst_y),
				  NUM2INT (NIL_P (dst_w) ? src_w : dst_w),	
				  NUM2INT (NIL_P (dst_h) ? src_h : dst_h));
    }

  return self;
}

static VALUE
rb_cogl_draw_buffer (int argc, VALUE *argv, VALUE self)
{
  VALUE target, offscreen;

  rb_scan_args (argc, argv, "11", &target, &offscreen);

  cogl_draw_buffer (NUM2UINT (target),
		    NIL_P (offscreen)
		    ? COGL_INVALID_HANDLE
		    : rb_cogl_offscreen_get_handle (offscreen));

  return Qnil;
}

void
rb_cogl_offscreen_init ()
{
  VALUE klass = rb_define_class_under (rbclt_c_cogl, "Offscreen",
				       rb_cObject);
  rb_c_cogl_offscreen = klass;

  rb_c_cogl_offscreen_error = rb_define_class_under (klass, "Error",
						     rb_eStandardError);

  rb_define_alloc_func (klass, rb_cogl_offscreen_allocate);

  rb_define_method (klass, "initialize", rb_cogl_offscreen_initialize, -1);
  rb_define_method (klass, "blit", rb_cogl_offscreen_blit, -1);
  rb_define_singleton_method (rbclt_c_cogl, "draw_buffer",
			      rb_cogl_draw_buffer, -1);
}
