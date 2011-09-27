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
require 'mkmf-gnome2'
require 'pkg-config'

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')

PKGConfig.have_package('glib-2.0') or show_fail
PKGConfig.have_package('clutter-1.0', 1, 6) or show_fail
PKGConfig.have_package('clutter-gst-1.0') or show_fail
PKGConfig.have_package('gstreamer-0.10') or show_fail

find_header("rbgobject.h", *$:) or show_fail

add_depend_package("clutter", "clutter", TOPDIR)

$objs = %w{ rbcluttergst.o rbcltgstvideosink.o rbcltgstvideotexture.o }

$INSTALLFILES = [ [ "clutter_gst.rb", "$(RUBYLIBDIR)" ] ]

create_makefile("clutter_gst")
