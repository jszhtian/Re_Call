#!/bin/sh
if [ -f "lib/$1" ]; then
	exit 0
fi
cd "src/$2"

CONFIGURE_CMD="./configure $GENERAL_LINK_OPTIONS --prefix=$PREFIX"
if [ "$#" -ge "3" ]; then
	CONFIGURE_CMD="$CONFIGURE_CMD $3"
fi
$CONFIGURE_CMD && $GNUMAKE && $GNUMAKE install
exit "$?"
