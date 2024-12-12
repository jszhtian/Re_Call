define(CHECK,`echo "Testing for $1..."
$1 --help >/dev/null 2>/dev/null
if [ "$?" -ne "0" ]; then
	$1 --version >/dev/null 2>/dev/null
	if [ "$?" -ne "0" ]; then
		echo Not found.
		exit 1
	fi
fi
')dnl
CHECK(`$GNUMAKE')
CHECK(`cmake')
CHECK(`gcc')
CHECK(`g++')
CHECK(`makeinfo')
CHECK(`tar')
#CHECK(`bzip2')
CHECK(`xz')
CHECK(`pkg-config')
exit 0