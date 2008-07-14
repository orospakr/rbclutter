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

#ifndef _RBCOGL_TEXTURE_H
#define _RBCOGL_TEXTURE_H

#include <cogl/cogl.h>

CoglHandle rb_cogl_texture_get_handle (VALUE obj);

#endif /* _RBCOGL_TEXTURE_H */
