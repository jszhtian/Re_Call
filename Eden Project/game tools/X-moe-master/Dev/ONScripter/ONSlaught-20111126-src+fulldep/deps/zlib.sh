#!/bin/sh
if [ -f "lib/libz.so" ]; then
	exit 0
fi
cd "src/zlib"

./configure $ZLIB_LINK_OPTIONS "--prefix=$PREFIX" && $GNUMAKE && $GNUMAKE install
exit "$?"
