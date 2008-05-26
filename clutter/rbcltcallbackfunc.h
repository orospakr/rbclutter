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

#ifndef _RBCLT_CALLBACK_FUNC_H
#define _RBCLT_CALLBACK_FUNC_H

typedef struct _RBCLTCallbackFunc RBCLTCallbackFunc;

void rbclt_callback_func_init ();

RBCLTCallbackFunc *rbclt_callback_func_new (VALUE proc);
VALUE rbclt_callback_func_invoke (RBCLTCallbackFunc *func, int arg_count,
				  const VALUE *argv);
void rbclt_callback_func_destroy (RBCLTCallbackFunc *func);

#endif /* _RBCLT_CALLBACK_FUNC_H */
