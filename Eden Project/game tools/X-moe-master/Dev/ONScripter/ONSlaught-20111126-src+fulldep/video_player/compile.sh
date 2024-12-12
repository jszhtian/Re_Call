#!/bin/sh
if [ "$GNUMAKE" = "" ]; then
	GNUMAKE="make"
fi
echo "We'll use "$GNUMAKE
export GNUMAKE
cd deps
m4 compile_libs.sh.m4 > compile_libs.sh
chmod +x compile_libs.sh
./compile_libs.sh $@
if [ "$?" -ne "0" ]; then
	echo One or more libraries failed to build.
	exit 1
fi
cp -rf lib/* ../../ONSlaught-bin
cd ..

echo "Compiling video player."
cd FFmpeg
cmake . >/dev/null 2>/dev/null
if [ "$?" -ne "0" ]; then
	echo cmaking failed.
	exit 1
fi
make >/dev/null 2>/dev/null
if [ "$?" -ne "0" ]; then
	echo making failed.
	exit 1
fi
if [ ! -f libvideo_player.so ]; then
	echo Building failed.
	exit 1
fi
cp libvideo_player.so ../../ONSlaught-bin/
cd ..
echo Building succeeded.
exit 0
