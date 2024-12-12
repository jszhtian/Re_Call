# Linux でPCMのドライバとして OSS を使用する場合、は -DUSE_OSS が必要

# OSS 以外にも ALSA(Advanced Linux Sound Architecture) と ESD(Enligtenment Sound Daemon)も
# 動くと思われるが、未テスト。その辺についてはxsystem35 の README.music を参照。

CFLAGS= -I.. $(LOCAL_DEF) -g -O2 -DHAVE_CONFIG_H -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -pthread -O2 # -ggdb -O0
CXXFLAGS	= $(CFLAGS)


CC              = gcc
CXX             = g++
LD              = g++
AR              = ar
RANLIB          = ranlib

all: libmusic.a nwatowav

@AUDIO_IO@.o: @AUDIO_IO@.c @ALSAMIX_H@

libmusic.a: config.h music.o koedec.o koedec_ogg.o wavfile.o movie.o nwatowav.o
	rm -f libmusic.a
	$(AR) clq libmusic.a music.o koedec.o koedec_ogg.o wavfile.o movie.o nwatowav.o
	$(RANLIB) libmusic.a

nwatowav: nwatowav.cc
	$(LD) -o nwatowav -DUSE_MAIN nwatowav.cc

config.h:
	ln -s ../config.h .

clean:
	rm -f *.o *.a *.core *.bak config.h nwatowav

