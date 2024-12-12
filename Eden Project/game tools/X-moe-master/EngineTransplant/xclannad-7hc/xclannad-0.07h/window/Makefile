
CC		= gcc
CXX		= g++
LD		= g++
AR		= ar
RANLIB		= ranlib

CFLAGS= -I.. $(LOCAL_DEF) -g -O2 -DHAVE_CONFIG_H -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -pthread -O2
CXXFLAGS	= $(CFLAGS)
LDFLAGS =  -lfreetype -lz -L/usr/lib -lSDL -lmad -lvorbisfile -lvorbis -logg -ljpeg -lpng -lz   -lX11 -lXext -L/usr/X11R6/lib -pthread

SRCS	= render.cc \
	event.cc \
	system.cc \
	picture.cc \
	widget.cc \
	button.cc \
	menuitem.cc \
	SDL_rotozoom.cc \
	rect.cc

OBJS	 = ${SRCS:.cc=.o}

all: libwindow.a test

libwindow.a: ${OBJS}
	rm -f libwindow.a
	${AR} clq libwindow.a ${OBJS}
	$(RANLIB) libwindow.a

clean:
	rm -f libwindow.a ${OBJS} *.bak *.core

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

.cc.o:
	$(CXX) -c $(CFLAGS) -o $@ $<

test: $(OBJS) test.o
	$(LD) -o test test.o $(OBJS) ../font/libfont.a ../system/libsystem.a $(LDFLAGS)

.png.bin:
	pngtopnm $<.png | ppmtopgm | dd bs=1 skip=15 > $@
.bin.txt: runlength
	./runlength $< > $@
