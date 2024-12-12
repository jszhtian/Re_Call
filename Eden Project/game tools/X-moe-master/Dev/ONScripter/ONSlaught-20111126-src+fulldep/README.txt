(Updated: 2011-03-14)

ONSlaught - An ONScripter clone with Unicode support.


                                  Introduction

I suck at writing so I'll just steal from ONScripter. I hope whoever's
maintaining it at the time doesn't mind.

<<Excerpt from the ONScripter-En README>>
  HISTORY

Naoki Takahashi's `NScripter' is a popular Japanese game engine used
for both commercial and free visual novels.  It attained popularity
due to its liberal terms of use and relative simplicity.  However, it
is closed source software and only available for Windows.

A number of cross-platform clones exist, and Ogapee's `ONScripter' is
the most popular of these.  Due to the ease with which it can be
modified to support languages other than Japanese, ONScripter has been
adopted by the visual novel localisation community as the engine of
choice for translated NScripter games; the patch to support English
even made it into the main ONScripter source code.

Nevertheless, the English-language community has found it convenient
to maintain its own branch of the code, and to enhance it in ways best
suited to the use we make of it.
<<The excerpt ends here.>>

However, ONScripter has one major flaw: It does not support Unicode. Like
ONScripter's maintainer said, each language community needs to keep its own
version of the engine. There's ONScripter-En, ONScripter-Ru, ONScripter-zh, etc.
Each of these versions modifies the original engine in such a way that they're
incompatible with each other.
From the point of view of software development, this is utterly retarded.
Instead of putting up with it and developing ONScripter-es, I decided to scrap
the entire code and write my own engine from scratch. The result of this effort
is ONSlaught.

ONSlaught's implicit and explicit goal is to supersede ONScripter, while at the
same time maintaining a simple design, and code as portable as possible.


REQUIREMENTS (compilation)

Development versions of the following libraries are required to compile (if
you're not running a POSIX-based system or this file appears in the "fulldep"
distribution, ignore this list):
SDL,
SDL_image,
libbz2,
freetype,
libFLAC,
libvorbisfile,
libvorbis,
libogg,
libjpeg,
libpng,
libtiff,
libmikmod,
libopenal,
libmpg123,
zlib.


ABOUT MIDI

MIDI playback is based on libtimidity, a software-based synthesizer. libtimidity needs sound
fonts in order to render files. I have a configuration ready to be used at
https://sourceforge.net/projects/onslaught-vn/files/Timidity%20config/timidity-config.7z/download
The directory needs to be in the same directory as the game data in order to
work. Technically, it needs to be in the engine's working directory, but since
the two will most often be the same...


PORTING

At the moment, ONSlaught binaries are only available for Windows 32-bit, but
since ONSlaught uses more or less the same libraries as ONScripter, it's
possible to port it to the same platforms ONScripter can be ported to. In
particular, MacOS X and above, PSP, and iPod are known to be possible options.
The engine was primarily written in Visual C++ 9.0 (2008), but it can also be
compiled with MinGW and GCC. GCC compilation has only been tested on versions 3
and above. Visual C++ 6.0 is not able to compile it. MinGW versions below 4.x
are not able to compile it.

At the moment, I'm looking for programmers with knowledge of iPod to port the
engine. If you possess any of these skills and are interested to contribute,
please refer to CONTACT INFORMATION in this document.
I'm also looking for PSP testers.


CONTACT INFORMATION

You may contact me at helios.vmg@gmail.com.


LICENCE

ONSlaught uses sources under a variety of licences. Refer to Licence.txt for
details.
