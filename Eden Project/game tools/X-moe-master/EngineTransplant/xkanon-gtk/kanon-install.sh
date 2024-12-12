# xkanon installer
# PREVINSTALLDIR すでに別バージョンがインストールされているかもしれないディレクトリ
# INSTALLDIR	インストール先
# FROMDIR	標準のインストールもと
# BINNAME	KANON の実行ファイル名

if [ $# -gt 0 ] ; then
	FROMDIR=$1
fi

# ディレクトリのチェック

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

# ファイルをコピー
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
	# 別バージョンが存在すれば、同じ pdt ファイルはコピーしないでいい
	tmpfile=$INSTALLDIR/pdt/tmp
	for fname in `cd $FROMDIR/pdt; ls` ; do
		prevfile=$PREVINSTALLDIR/pdt/$fname
		fromfile=$FROMDIR/pdt/$fname
		tofile=$INSTALLDIR/pdt/$fname
		rm -f $tofile
		# PDT ファイルが存在して、symbolic link でもないとき
		if [ -r $prevfile -a ! -h $prevfile ] ; then
			rm -f $tmpfile
			cp $fromfile $tmpfile
			# ファイルを比較し、同じなら symbolic link にする
			cmp -s $prevfile $tmpfile
			if [ X$? = X0 ]; then
				ln -s $prevfile $tofile
			else
				mv $tmpfile $tofile
			fi
		else
			# PDT ファイルが存在しないなら、単純にコピー
			cp $fromfile $tofile
		fi
	done
	rm -f $tmpfile
fi
echo "'$GAME' install finisihed !"
