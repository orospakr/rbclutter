#!/usr/bin/ruby

# This is needed to ensure that clutter.so is loaded before
# clutter_gst.so so that it can reference symbols in clutter.so

require 'clutter'
require 'clutter_gst.so'
