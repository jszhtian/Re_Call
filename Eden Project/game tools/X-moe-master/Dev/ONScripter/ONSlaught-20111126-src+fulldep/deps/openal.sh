#!/bin/sh
if [ -f lib/libopenal.so ]; then
	exit 0
fi
cd src/openal
if [ "$STATIC_BUILD" = "1" ]; then
	BUILD_TYPE="-DLIBTYPE=STATIC"
else
	BUILD_TYPE=""
fi
cmake "-DCMAKE_INSTALL_PREFIX=$PREFIX" "$BUILD_TYPE" . && $GNUMAKE && $GNUMAKE install
if [ "$?" -ne "0" ]; then
	exit 1
fi
cd ../../include
mv AL/* .
exit 0
