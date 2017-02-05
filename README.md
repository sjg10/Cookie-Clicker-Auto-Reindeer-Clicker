Introduction
============

A cli activated C-based tool utilising the imageMagick Wand API and X11 headers to look for and then autoclick Reindeer!

Build Process
=============

To build call make on the makefile (with no parameters) in the build directory. An executable, clicker, will be created there.

Plan
====

- Use imageMagick Wand to capture (or even look in GPU memory) an image [like so: http://stackoverflow.com/questions/13219710/taking-a-screenshot-using-imagemagick-api]
- Periodically capture new images
- Look exclusively in the central pane and find a diff (using multi-threads and skipping pixels for speed up]
- If diff exists, click it (maybe with offset depending on speed) using X11 tools: http://stackoverflow.com/questions/16714928/sending-keystrokes-to-a-x-window 
