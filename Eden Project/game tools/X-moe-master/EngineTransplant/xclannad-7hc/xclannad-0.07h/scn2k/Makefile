
CC		= gcc
CXX		= g++
LD		= gcc
AR		= ar
RANLIB		= ranlib

CFLAGS=  -I.. $(LOCAL_DEF) -g -O0 -DHAVE_CONFIG_H -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -pthread 
CXXFLAGS	= $(CFLAGS)
LDFLAGS =  -ljpeg -L/usr/lib -lSDL  -lSDL_mixer -L/usr/lib -lsmpeg -L/usr/lib -lSDL -lfreetype -lz -lmad -lvorbisfile -lvorbis -logg -ljpeg -lpng -lz   -lX11 -lXext -L/usr/X11R6/lib -pthread

SRCS	 = scn2k_cmd.cc scn2k_text.cc scn2k_grp.cc scn2k_impl.cc

OBJS	 = ${SRCS:.cc=.o}

all: test libscn2k.a scn2kdump gandump

test: test.o $(OBJS)
	$(LD) -o test test.o $(OBJS) ../window/libwindow.a ../music2/libmusic.a ../system/libsystem.a ../font/libfont.a  $(LDFLAGS)

scn2kdump: scn2kdump.o
	$(LD) -o scn2kdump scn2kdump.o ../system/libsystem.a $(LDFLAGS)

gandump: gandump.o
	$(LD) -o gandump gandump.o $(LDFLAGS)

libscn2k.a: ${OBJS}
	rm -f libscn2k.a
	${AR} clq libscn2k.a ${OBJS}
	$(RANLIB) libscn2k.a

clean:
	rm -f test libscn2k.a ${OBJS} *.bak *.core gandump scn2kdump

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

.cc.o:
	$(CXX) -c $(CFLAGS) -o $@ $<

