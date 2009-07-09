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

#ifndef _RBCLUTTER_H
#define _RBCLUTTER_H

#include <ruby.h>
#include <glib/gtypes.h>
#include <clutter/clutter.h>
#include <cogl/cogl.h>
/* #include <clutter/clutter-fixed.h>
#include <clutter/clutter-units.h>
#include <clutter/clutter-main.h> */

#define RBCLUTTER_MAJOR_VERSION 0
#define RBCLUTTER_MINOR_VERSION 8
#define RBCLUTTER_MICRO_VERSION 0

extern VALUE rbclt_c_clutter;
extern VALUE rbclt_c_clutter_error;
extern VALUE rbclt_c_cogl;

void rbclt_initialize_unowned (VALUE obj, gpointer gobj);
guint8 rbclt_num_to_guint8 (VALUE val);
guint16 rbclt_num_to_guint16 (VALUE val);
CoglFixed rbclt_num_to_fixed (VALUE val);
/* ClutterUnit rbclt_num_to_units (VALUE val); */
/* ClutterAngle rbclt_num_to_angle (VALUE angle); */
GType rbclt_connect_flags_get_type (void) G_GNUC_CONST;

typedef ClutterInitError (* RBCLTInitFunc) (int *argc, char ***argv);

VALUE rbclt_call_init_func (int argc, VALUE *argv, RBCLTInitFunc func);

#endif /* _RBCLUTTER_H */
