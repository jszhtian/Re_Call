#!/bin/sh
autoheader259
aclocal19 -I . -I /usr/local/share/aclocal
autoconf259
sed -e "s@HAVE_LC_MESSAGES',@HAVE_LC_MESSAGES@" -e "s@VERSION'@VERSION@" config.h.in>config.h.in.new
cp config.h.in.new config.h.in
rm -f config.h.in.new
# gcc 3.0.2 �Ѥν���
# gtkmm �� autoconf ���Ѥǥ��顼���Ф�Τ�
# ����Ū�˥��顼�θ��ˤʤ�Ԥ���
sed -e 's/^extern "C" void exit(int);//' configure > configure.new
cp configure.new configure
rm -f configure.new
./configure $*
