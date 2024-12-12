(Updated: 2011-03-14)

                                  INTRODUCTION

The purpose of this package is to simplify compilation on UNIX systems as much
as possible, since I've confirmed that, given enough complexity, it's
practically impossible to produce a binary that will run on any Linux
distribution.
This package contains all cross-platform dependencies and sub-dependencies.
Namely:

  bz2lib
  FFmpeg
  freetype
  libflac
  libjpeg
  libmikmod
  libmpg123
  libogg
  libpng
  libvorbis
  OpenAL
  SDL
  SDL_image
  libtiff
  zlib

Running the compilation script doesn't install anything on the system, and
doesn't require root privileges. It only compiles and installs everything to
deps/.


                                  REQUIREMENTS

* GNU make (Required to build the dependencies. The engine itself builds just
  fine with other makes)
* cmake
* gcc
* g++
* makeinfo
* tar
* xz
* pkg-config
NOTE: versions 4.x and above of gcc and g++ are preferred, but 3.x should also
work. gcc and g++ should be the same versions.
Development libraries:
* Graphics:
  * Xlib (usually libx11-dev in repositories)
  * X11 miscellaneous extensions library (libxext-dev)
  * Direct frame buffer graphics library (libdirectfb-dev)
* Audio:
  * ALSA (libasound2-dev),
    or ESD (libesd0-dev),
    or PulseAudio (libpulse-dev)
The above are confirmed dependencies for PC systems. I haven't tried building
the engine for other platforms.


                                NOTE: BSD USERS

(The following paragraph is somewhat old. I haven't tried building FFmpeg under
BSD for a long time, so I don't know if it still applies.)
Unfortunately, FFmpeg -- and by extension, the video player -- doesn't compile
under BSD. Apparently, there's a broken system header that makes the build
process fail. There isn't much I can do about this.


                                  INSTRUCTIONS

1. The POSIX version of 7z doesn't set the execute flag on uncompressed files.
   You will probably have to run
     chmod -R +x *
   to be able to compile properly.
2. If you're using a BSD and "make" doesn't refer to GNU make (run
   make --version to check), you'll have to set GNUMAKE. For example:
     GNUMAKE=/usr/gnu/bin/make
     export GNUMAKE
3. You can statically link dependencies by setting STATIC_BUILD:
     STATIC_BUILD=1
     export STATIC_BUILD
   Linking statically simplifies the execution mechanism, but it doesn't quite
   work for all systems. It's probably worth a try, though.
4. If you have more than one CPU/core, you may want to use all of them to build
   faster. You can use the MAKEOPTS variable to tell make how many sources to
   compile in parallel. For example:
     MAKEOPTS="-j2"
     export MAKEOPTS
   This can significantly reduce the compilation time.
5. ./compile.sh
   Continue reading once building has succeeded.
6. If necessary, you can now build the video player.
     cd video_player
     ./compile.sh
   Its script will use environment variables from steps 2-4 if you set them.
   In some cases, FFmpeg has built incorrectly when using Assembly
   optimizations. This has caused segmentation faults when attempting to play a
   video file. If this is happening to you, set NO_ASM to 1 and rebuild.


                                   HOW TO RUN

STATIC BUILD WITHOUT VIDEO PLAYER
The executable file "ONSlaught" is all you need to run the engine. I recommend
placing it in a directory where it can be ran from anywhere, such as /usr/bin,
or ~/bin, if you have that in your PATH environment variable.

STATIC BUILD WITH VIDEO PLAYER
This directory contains ONSlaught.sh and ONSlaught-bin/. You can delete
everything in ONSlaught-bin/ except for libvideo_player.so and "ONSlaught".
ONSlaught.sh and ONSlaught-bin/ have to be in the same directory. The directory
structure would look like this:
  ?/ONSlaught.sh
  ?/ONSlaught-bin/ONSlaught
  ?/ONSlaught-bin/libvideo_player.so
Run ONSlaught.sh to run the engine.
ONSlaught.sh sets a variable that tells the system where to find
libvideo_player.so.
Alternatively, you can move libvideo_player.so to /usr/lib (or wherever your
system stores libraries). Then the structure would
look like this:
  ?/ONSlaught
  /usr/lib/libvideo_player.so
Then you wouldn't need ONSlaught.sh.

DYNAMIC BUILD WITH OR WITHOUT VIDEO PLAYER
This directory contains ONSlaught.sh and ONSlaught-bin/. You can delete
everything in ONSlaught-bin/ that ends with *.a or *.la
ONSlaught.sh and ONSlaught-bin/ have to be in the same directory. The directory
structure would look like this:
  ?/ONSlaught.sh
  ?/ONSlaught-bin/ONSlaught
  ?/ONSlaught-bin/*.so*
Run ONSlaught.sh to run the engine.
