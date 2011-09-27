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

/* http://docs.clutter-project.org/docs/clutter/stable/ClutterBoxLayout.html */

static VALUE
rbclt_box_layout_initialize (VALUE self)
{
  rbclt_initialize_unowned (self, clutter_box_layout_new ());
  return Qnil;
}

/* clutter_box_layout_pack   */
/* clutter_box_layout_set_alignment  */
/* clutter_box_layout_get_alignment  */
/* clutter_box_layout_set_expand    */
/* clutter_box_layout_get_expand   */
/* clutter_box_layout_set_fill   */
/* clutter_box_layout_get_fill   */
/* clutter_box_layout_set_easing_mode  */
/* clutter_box_layout_get_easing_mode  */

static VALUE
rbclt_box_layout_pack (VALUE self, VALUE actor, VALUE expand, VALUE x_fill, VALUE y_fill, VALUE x_align, VALUE y_align)
{
  ClutterBoxLayout *layout = CLUTTER_BOX_LAYOUT (RVAL2GOBJ (self));
  ClutterActor *g_actor = CLUTTER_ACTOR (RVAL2GOBJ (self));

  clutter_box_layout_pack (layout, g_actor, RVAL2CBOOL (expand), RVAL2CBOOL (x_fill), RVAL2CBOOL (y_fill), RVAL2GENUM (x_align, CLUTTER_TYPE_BOX_ALIGNMENT), RVAL2GENUM (y_align, CLUTTER_TYPE_BOX_ALIGNMENT));
  return Qnil;
}


void
rbclt_box_layout_init ()
{
  VALUE klass = G_DEF_CLASS (CLUTTER_TYPE_BOX_LAYOUT, "BoxLayout", rbclt_c_clutter);

  rb_define_method (klass, "initialize", rbclt_box_layout_initialize, 0);
  rb_define_method (klass, "pack", rbclt_box_layout_pack, 6);
}
