PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man
X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib
XINERAMALIBS  = -lXinerama
XINERAMAFLAGS = -DXINERAMA
FREETYPELIBS = -lfontconfig -lXft
FREETYPEINC = /usr/include/freetype2
INCS = -I${X11INC} -I${FREETYPEINC}
LIBS = -L${X11LIB} -lX11 ${XINERAMALIBS} ${FREETYPELIBS}
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200809L -DVERSION=\"${VERSION}\" ${XINERAMAFLAGS}
CFLAGS   = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}
CC = gcc


DESDIR = ./out/
SRCDIR = ./src/

SRC = ${SRCDIR}util.c ${SRCDIR}mywm.c 
OBJ = ${SRC:.c=.o}

all:  mywm build-clean

.c.o: ${CC} -c ${CFLAGS} $<

mywm: ${OBJ} 
	${CC} -o ${DESDIR}$@ ${OBJ} ${LDFLAGS}

build-clean:
	rm -f ${SRCDIR}*.o	

.PHONY: all  
