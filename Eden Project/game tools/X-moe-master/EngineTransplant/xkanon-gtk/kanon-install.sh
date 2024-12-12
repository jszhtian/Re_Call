# xkanon installer
# PREVINSTALLDIR ���Ǥ��̥С�����󤬥��󥹥ȡ��뤵��Ƥ��뤫�⤷��ʤ��ǥ��쥯�ȥ�
# INSTALLDIR	���󥹥ȡ�����
# FROMDIR	ɸ��Υ��󥹥ȡ�����
# BINNAME	KANON �μ¹ԥե�����̾

if [ $# -gt 0 ] ; then
	FROMDIR=$1
fi

# �ǥ��쥯�ȥ�Υ����å�

if [ ! -r $FROMDIR/gameexe.ini ]; then
	echo "Cannot find file $FROMDIR/gameexe.init ; may be invalid directory." 2>&1
	exit;
fi

if [ ! -d $FROMDIR/dat ]; then
	echo "Cannot find directory $FROMDIR/dat ; may be invalid directory." 2>&1
	exit;
fi

if [ ! -d $FROMDIR/pdt ]; then
	echo "Cannot find directory $FROMDIR/pdt ; may be invalid directory." 2>&1
	exit;
fi

if [ ! -r $FROMDIR/$BINNAME ] ; then
	echo "Cannot find executable $FROMDIR/$BINNAME ; may be invalid version." 2>&1
	exit;
fi

if [ ! -d $INSTALLDIR ] ; then
	mkdir -p $INSTALLDIR
fi

# �ե�����򥳥ԡ�
echo Copying root...
cp $FROMDIR/* $INSTALLDIR > /dev/null 2>&1
echo Copying dat...
mkdir -p $INSTALLDIR/dat
cp $FROMDIR/dat/* $INSTALLDIR/dat/ > /dev/null 2>&1

echo Copying pdt...
mkdir -p $INSTALLDIR/pdt
if [ "X$PREVINSTALLDIR" = X -o ! -d $PREVINSTALLDIR ] ; then
	cp $FROMDIR/pdt/* $INSTALLDIR/pdt/ > /dev/null 2>&1
else
	# �̥С������¸�ߤ���С�Ʊ�� pdt �ե�����ϥ��ԡ����ʤ��Ǥ���
	tmpfile=$INSTALLDIR/pdt/tmp
	for fname in `cd $FROMDIR/pdt; ls` ; do
		prevfile=$PREVINSTALLDIR/pdt/$fname
		fromfile=$FROMDIR/pdt/$fname
		tofile=$INSTALLDIR/pdt/$fname
		rm -f $tofile
		# PDT �ե����뤬¸�ߤ��ơ�symbolic link �Ǥ�ʤ��Ȥ�
		if [ -r $prevfile -a ! -h $prevfile ] ; then
			rm -f $tmpfile
			cp $fromfile $tmpfile
			# �ե��������Ӥ���Ʊ���ʤ� symbolic link �ˤ���
			cmp -s $prevfile $tmpfile
			if [ X$? = X0 ]; then
				ln -s $prevfile $tofile
			else
				mv $tmpfile $tofile
			fi
		else
			# PDT �ե����뤬¸�ߤ��ʤ��ʤ顢ñ��˥��ԡ�
			cp $fromfile $tofile
		fi
	done
	rm -f $tmpfile
fi
echo "'$GAME' install finisihed !"
