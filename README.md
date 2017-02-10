Introduction
============

Cookie Clicker is a Javascript powered browser game. One mode of it involves clicking on reindeer as they appear and mov across the screen.
They spawn rarely and some game achievements require 200 clicks.
This is Cookie Clicker Auto Reindeer Clicker. It is a CLI C-based tool, utilising the imageMagick Wand API and X11 headers, to scan the Cookie Clicker window for reindeer, and then autoclick them.

License
=======

This is freely available under the GPL v3.

Prerequisites
=============

- The system must have libmagickwand tools installed.
   On Ubuntu this can be installed with `apt-get install libmagickwand-dev`, or get the ImageMagick development rpm's from https://www.imagemagick.org/script/download.php and install these.
- The system must have x11 utils, specifically xwininfo (most distros include this).

Build Process
=============

To build call make on the makefile (with no parameters) in the build directory. An executable, clicker, will be created there.
- If libmagickwand tools have been installed and are not found by make then call `export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig` before make to find them (as per https://www.imagemagick.org/script/magick-wand.php).

Plan
====

- Use imageMagick Wand to capture (or even look in GPU memory) an image [like so: http://stackoverflow.com/questions/13219710/taking-a-screenshot-using-imagemagick-api]
- Periodically capture new images
- Look exclusively in the central pane and find a diff (using multi-threads and skipping pixels for speed up]
- If diff exists, click it (maybe with offset depending on speed) using X11 tools: http://stackoverflow.com/questions/16714928/sending-keystrokes-to-a-x-window 
