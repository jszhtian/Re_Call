#!/bin/sh
autoheader259
aclocal19 -I . -I /usr/local/share/aclocal
autoconf259
sed -e "s@HAVE_LC_MESSAGES',@HAVE_LC_MESSAGES@" -e "s@VERSION'@VERSION@" config.h.in>config.h.in.new
cp config.h.in.new config.h.in
rm -f config.h.in.new
# gcc 3.0.2 用の修正
# gtkmm の autoconf が変でエラーが出るので
# 強制的にエラーの元になる行を削除
sed -e 's/^extern "C" void exit(int);//' configure > configure.new
cp configure.new configure
rm -f configure.new
./configure $*
