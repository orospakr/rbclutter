# Ruby bindings for the Clutter 'interactive canvas' library.
# Copyright (C) 2007  Neil Roberts
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301  USA
require 'mkmf'
require 'pkg-config'

PKGConfig.have_package('glib-2.0') or show_fail
PKGConfig.have_package('clutter-0.4') or show_fail
PKGConfig.have_package('clutter-gtk-0.4') or show_fail
PKGConfig.have_package('gtk+-2.0') or show_fail

find_header("rbgobject.h", *$:) or show_fail
find_header("rbgtk.h", *$:) or show_fail

$objs = %w{ rbcluttergtk.o }

create_makefile("clutter_gtk")
