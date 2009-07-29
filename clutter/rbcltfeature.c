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

#include <rbgobject.h>
#include <clutter/clutter.h>

#include "rbclutter.h"

static VALUE
rbclt_feature_features (VALUE self)
{
  return GFLAGS2RVAL (clutter_feature_get_all (), CLUTTER_TYPE_FEATURE_FLAGS);
}

static VALUE
rbclt_feature_feature_available (VALUE self, VALUE feature)
{
  return clutter_feature_available (RVAL2GFLAGS (feature, CLUTTER_TYPE_FEATURE_FLAGS))
    ? Qtrue : Qfalse;
}

void
rbclt_feature_init ()
{
  G_DEF_CLASS (CLUTTER_TYPE_FEATURE_FLAGS, "FeatureFlags", rbclt_c_clutter);
  G_DEF_CONSTANTS (rbclt_c_clutter, CLUTTER_TYPE_FEATURE_FLAGS, "CLUTTER_");

  rb_define_singleton_method (rbclt_c_clutter, "features", rbclt_feature_features, 0);
  rb_define_singleton_method (rbclt_c_clutter, "feature_available?",
                              rbclt_feature_feature_available, 1);
}
