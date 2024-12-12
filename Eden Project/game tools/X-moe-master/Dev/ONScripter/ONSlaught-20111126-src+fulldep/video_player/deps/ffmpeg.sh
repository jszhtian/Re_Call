#!/bin/sh
if [ -f "lib/libavcodec.a" ]; then
	exit 0
fi
cd "src/ffmpeg"

FFMPEG_OPTIONS=""

if [ "$STATIC_BUILD" = "1" ]; then
	FFMPEG_OPTIONS="$FFMPEG_OPTIONS --disable-shared"
else
	FFMPEG_OPTIONS="$FFMPEG_OPTIONS --enable-shared"
fi

if [ "$NO_ASM" = "1" ]; then
	FFMPEG_OPTIONS="$FFMPEG_OPTIONS --disable-asm --disable-yasm"
fi

./configure $FFMPEG_OPTIONS --enable-static --disable-doc --disable-ffmpeg --disable-ffplay --disable-ffprobe --disable-ffserver --enable-postproc --enable-swscale --enable-gpl --enable-version3 --enable-nonfree "--prefix=$PREFIX" && $GNUMAKE && $GNUMAKE install
exit "$?"
