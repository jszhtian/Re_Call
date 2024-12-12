#!/bin/sh
if [ "$GNUMAKE" = "" ]; then
	GNUMAKE="make"
fi
echo "We'll use "$GNUMAKE
export GNUMAKE
echo "Testing for m4..."
m4 --version >/dev/null 2>/dev/null
if [ "$?" -ne "0" ]; then
	echo Not found.
	exit 1
fi
if [ -f test_userland.sh.m4 ]; then
	m4 test_userland.sh.m4 > test_userland.sh
	chmod +x test_userland.sh
	./test_userland.sh
	A=$?
	rm test_userland.sh
	if [ "$A" -ne "0" ]; then
		exit 1
	fi
	rm test_userland.sh.m4
fi
GNUMAKE="$GNUMAKE $MAKEOPTS"
export GNUMAKE

if [ -f check_large_deps.sh.m4 ]; then
	m4 check_large_deps.sh.m4 > check_large_deps.sh
	rm check_large_deps.sh.m4
	chmod +x check_large_deps.sh
fi
if [ -f check_large_deps.sh ]; then
	./check_large_deps.sh
	if [ "$?" -ne "0" ]; then
		echo "One or more dependencies weren't found. ONSlaught may not build properly."
	else
		echo All dependencies appear to be available.
		rm check_large_deps.sh
	fi
fi

SHARED_LINK_OPTIONS="--enable-shared --enable-static"
STATIC_LINK_OPTIONS="--disable-shared --enable-static"
if [ "$STATIC_BUILD" = "1" ]; then
	echo Statically building and linking all dependendencies.
	ZLIB_LINK_OPTIONS=
	GENERAL_LINK_OPTIONS=$STATIC_LINK_OPTIONS
else
	echo Dynamically building and linking all dependendencies.
	ZLIB_LINK_OPTIONS="--shared"
	GENERAL_LINK_OPTIONS=$SHARED_LINK_OPTIONS
fi
IMAGE_LINK_OPTIONS=$GENERAL_LINK_OPTIONS
export GENERAL_LINK_OPTIONS
export STATIC_LINK_OPTIONS
export IMAGE_LINK_OPTIONS
export ZLIB_LINK_OPTIONS

if [ -f ONSlaught ]; then
	echo ONSlaught has already been built. Skipping.
	echo To rebuild, run
	echo "    rm ONSlaught"
	exit 0
fi
cd deps
m4 compile_libs.sh.m4 > compile_libs.sh
chmod +x compile_libs.sh
./compile_libs.sh
if [ "$?" -ne "0" ]; then
	echo One or more libraries failed to build.
	echo "See the corresponding deps/*.stderr.txt file to find out why."
	exit 1
fi
rm compile_libs.sh
rm -rf ../ONSlaught-bin 2>/dev/null
cp -r lib ../ONSlaught-bin
cd ..
echo "Compiling ONSlaught..."
OSTDOUT="ONSlaught.stdout.txt"
OSTDERR="ONSlaught.stderr.txt"
cmake . >> $OSTDOUT 2>> $OSTDERR
if [ "$?" -ne "0" ]; then
	echo cmaking failed.
	exit 1
fi
make $MAKEOPTS >> $OSTDOUT 2>> $OSTDERR
if [ "$?" -ne "0" ]; then
	echo making failed.
	exit 1
fi
if [ ! -f ONSlaught ]; then
	echo Building failed.
	exit 1
fi
cp ONSlaught ONSlaught-bin
cp libplugin.so ONSlaught-bin
echo "Building succeeded. See README-fulldep for more info."
exit 0
