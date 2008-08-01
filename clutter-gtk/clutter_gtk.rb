#!/usr/bin/ruby

# This is needed to ensure that clutter.so is loaded before
# clutter_gtk.so so that it can reference symbols in clutter.so

require 'clutter'
require 'clutter_gtk.so'
