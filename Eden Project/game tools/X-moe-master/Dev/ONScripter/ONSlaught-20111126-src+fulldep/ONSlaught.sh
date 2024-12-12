#!/bin/sh
LOCATION="`dirname $0`"
if [ $LOCATION = "." ]; then
	LOCATION="$PWD"
fi
LD_LIBRARY_PATH="$LOCATION/ONSlaught-bin"
export LD_LIBRARY_PATH
"$LOCATION/ONSlaught-bin/ONSlaught" $@
