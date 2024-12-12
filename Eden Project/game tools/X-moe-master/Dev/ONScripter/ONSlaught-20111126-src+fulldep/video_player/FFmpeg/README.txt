
                                  Introduction

This sub-project is a small video player as an optional dynamically loaded
plugin for ONSlaught. It is the main part of the implementation of the avi and
mpegplay commands.
It is an optional component because the dependencies would increase the code
size too much (around 6 MiB, or four times the current code size), considering
how rarely are those two commands used.


                        Supported containers and codecs

Thanks to FFmpeg, the player supports many containers and codecs without any
additional dependencies. See Formats.txt for a complete list.
Note: This list includes the non-free modules, which aren't included in the
pre-built Windows binaries.


                              Usage under Windows

The DLL should be placed in the same directory as the executable.
The pre-built binaries, including the DLL and the MinGW import libraries, don't
include the non-free modules.
You can get new FFmpeg Windows nightly builds from
http://ffmpeg.arrozcru.org/autobuilds/
and new OpenAL Windows builds from
http://connect.creativelabs.com/openal/Downloads/Forms/AllItems.aspx
But you might need to recompile the plugin.


                                Usage under UNIX

The build script should have taken care of everything.
