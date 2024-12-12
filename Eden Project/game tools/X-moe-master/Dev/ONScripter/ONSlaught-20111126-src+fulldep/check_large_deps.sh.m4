define(CHECK,`pkg-config $1 >/dev/null 2>/dev/null
if [ "$?" -ne "0" ]; then
	echo WARNING: "$1 wasn''`t found by pkg-config."
	RET=1
fi
')dnl
define(CHECK_OPTIONAL,`if [ "$$2" -eq "0" ]; then
	pkg-config $1 >/dev/null 2>/dev/null
	if [ "$?" -eq "0" ]; then
		$2=1
	fi
fi
')dnl
RET=0
CHECK(`x11')
CHECK(`xext')
CHECK(`directfb')
SOUND=0
CHECK_OPTIONAL(`alsa',`SOUND')
CHECK_OPTIONAL(`esound',`SOUND')
CHECK_OPTIONAL(`libpulse',`SOUND')
if [ "$SOUND" -eq "0" ]; then
	echo Neither ALSA nor ESD nor PulseAudio were found by pkg-config.
	RET=1
fi
exit $RET
