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

#ifndef _RBCLT_ALPHA_FUNC_H
#define _RBCLT_ALPHA_FUNC_H

#include <clutter/clutter.h>

void rbclt_alpha_func_init (VALUE alpha_klass);

void rbclt_alpha_func_from_rb_value (VALUE func,
                                     ClutterAlphaFunc *func_ret,
                                     gpointer *data,
                                     GDestroyNotify *notify);

#endif /* _RBCLT_ALPHA_FUNC_H */
