# Ruby bindings for the Clutter 'interactive canvas' library.
# Copyright (C) 2007-2008  Neil Roberts
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
PKGConfig.have_package('clutter-0.8') or show_fail

find_header("rbgobject.h", *$:) or show_fail

BOXED_TYPES = %w{ color geometry knot vertex actorbox perspective }

$objs = %w{ rbclutter.o rbcltactor.o rbcltalpha.o rbcltbehaviour.o rbcltbehaviourpath.o } \
+ %w{ rbcltcontainer.o rbcltgroup.o rbcltlabel.o rbcltmain.o rbcltstage.o rbclttexture.o } \
+ %w{ rbclttimeline.o rbcltevent.o rbcltkeysyms.o rbcltbehaviourscale.o } \
+ %w{ rbcltbehaviourbspline.o rbcltbehaviourdepth.o rbcltbehaviourellipse.o } \
+ %w{ rbcltbehaviouropacity.o rbcltbehaviourrotate.o rbcltalphafunc.o rbclteffect.o } \
+ %w{ rbcltentry.o rbcltrectangle.o rbcltclonetexture.o rbcltfeature.o rbcltbackend.o } \
+ %w{ rbcltmedia.o rbcltshader.o rbcltcallbackfunc.o rbcltframesource.o } \
+ %w{ rbcltstagemanager.o rbcltchildmeta.o rbcltscript.o rbcltscore.o } \
+ %w{ rbcltlistmodel.o rbcltmodel.o }

$objs += %w{ rbcogl.o rbcoglconsts.o rbcogltexture.o rbcoglprimitives.o } \
+ %w{ rbcoglshader.o rbcoglprogram.o rbcogloffscreen.o }

# Add the boxed types to the object files list
BOXED_TYPES.each { |bt| $objs << "rbclt#{bt}.o" }
# Add the boxed type source files to the clean list
BOXED_TYPES.each { |bt| $cleanfiles << "rbclt#{bt}.c" }
# Add rbcltevent.c to the clean list
$cleanfiles << "rbcltevent.c"

create_makefile("clutter")

File.open("Makefile", "a") do |makefile|
  # Append extra rules to generate the boxed types
  BOXED_TYPES.each do |boxed_type|
    makefile.print("\nrbclt#{boxed_type}.c : rbclt#{boxed_type}.c.in mkboxedtype.rb\n" \
                   "\truby mkboxedtype.rb rbclt#{boxed_type}.c.in\n")
  end

  # Append an extra rule to make rbcltevent.c
  makefile.print("\nrbcltevent.c : mkrbcltevent.rb rbcltevent.c.in\n" \
                 "\truby mkrbcltevent.rb\n")
end
