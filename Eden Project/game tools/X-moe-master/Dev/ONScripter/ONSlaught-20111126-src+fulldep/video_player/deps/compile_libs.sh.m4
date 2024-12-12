define(LIB,`echo Compiling $1...
./$2.sh >$2.stdout.txt 2>$2.stderr.txt
if [ "$?" -ne "0" ]; then
	echo Failed.
	exit 1
fi')dnl
`#'!/bin/sh
PREFIX="$PWD"
CFLAGS="-w -I$PREFIX/include"
LDFLAGS="-L$PREFIX/lib"
LIBS="$LDFLAGS"
export PREFIX
export CFLAGS
export LDFLAGS
export LIBS

cd src

if [ -f ffmpeg.tar.xz ]; then
	echo Unpacking sources...
	xz -d ffmpeg.tar.xz
	tar -xf ffmpeg.tar
	rm ffmpeg.tar
fi

cd ..

LIB(`FFmpeg',`ffmpeg')
