#!/bin/sh

if [ -f lib/libbz2.a ]; then
	exit 0
fi
cd src/bzip2
make
if [ "$?" -ne "0" ]; then
	exit 1
fi
cp libbz2.a "$PREFIX/lib"
cp *.h "$PREFIX/include"
cd ../..
exit 0
