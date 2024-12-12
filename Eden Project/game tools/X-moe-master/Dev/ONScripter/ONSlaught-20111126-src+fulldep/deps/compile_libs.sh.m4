define(LIB,`echo Compiling $1...
./$2.sh >$2.stdout.txt 2>$2.stderr.txt
if [ "$?" -ne "0" ]; then
	echo Failed.
	exit 1
fi')dnl
define(LIB_COMMON,`echo Compiling $1...
./common.sh $3 $4 >$2.stdout.txt 2>$2.stderr.txt
if [ "$?" -ne "0" ]; then
	echo Failed.
	exit 1
fi
')dnl
define(LIB_COMMON2,`echo Compiling $1...
./common.sh $3 $4 $5 >$2.stdout.txt 2>$2.stderr.txt
if [ "$?" -ne "0" ]; then
	echo Failed.
	exit 1
fi
')dnl
`#'!/bin/sh
PREFIX="$PWD"
export PREFIX

if [ -f src.tar.xz ]; then
	echo Unpacking sources...
	xz -d src.tar.xz
	tar -xf src.tar
	rm src.tar
	if [ -f include.tar ]; then
		echo Unpacking headers...
		tar -xf include.tar
		rm include.tar
		echo Creating directories.
	else
		echo Creating directories.
		mkdir include
	fi
	mkdir bin
	mkdir lib
fi

`#' Compile zlib before anything else or the environment variables will make
`#' the moron crash and burn.
LIB(`zlib',`zlib')

CFLAGS="-w -I$PREFIX/include"
LDFLAGS="-L$PREFIX/lib"
LIBS="$LDFLAGS"
export CFLAGS
export LDFLAGS
export LIBS

LIB(`bz2lib',`bzip2')
LIB(`OpenAL',`openal')
LIB_COMMON2(`FreeType',`freetype',`libfreetype.a',`freetype',`"--without-zlib"')
LIB_COMMON2(`libjpeg',`libjpeg',`libjpeg.a',`jpeg',`"$IMAGE_LINK_OPTIONS"')
LIB_COMMON(`libogg',`libogg',`libogg.a',`libogg')
LIB_COMMON2(`mpg123',`mpg123',`libmpg123.a',`mpg123',`"--with-optimization=4"')
LIB_COMMON(`libmikmod',`mikmod',`libmikmod.a',`libmikmod')
LIB_COMMON2(`libpng',`libpng',`libpng.a',`libpng',`"$IMAGE_LINK_OPTIONS"')
LIB_COMMON2(`libtiff',`libtiff',`libtiff.a',`tiff',`"$IMAGE_LINK_OPTIONS"')
LIB_COMMON2(`libflac',`libflac',`libFLAC.a',`libflac',`"--with-ogg=$PREFIX"')
LIB_COMMON2(`libvorbis',`vorbis',`libvorbis.a',`libvorbis',`"--with-ogg=$PREFIX"')
LIB_COMMON2(`SDL',`SDL',`libSDL.a',`SDL',`"--disable-audio --disable-video-svga"')
if [ "$STATIC_BUILD" = "1" ]; then
	EXTRA_IMAGE_OPTIONS="--disable-jpg-shared --disable-png-shared --disable-tif-shared"
else
	EXTRA_IMAGE_OPTIONS=""
fi
LIB_COMMON2(`SDL_image',`SDL_image',`libSDL_image.a',`SDL_image',`"--with-sdl-prefix=$PREFIX $EXTRA_IMAGE_OPTIONS"')
