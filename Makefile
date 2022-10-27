
VERSION = 1.0

# X11
X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# Xinerama
XINERAMALIBS  = -lXinerama
XINERAMAFLAGS = -DXINERAMA

# freetype
FREETYPELIBS = -lfontconfig -lXft
FREETYPEINC = /usr/include/freetype2

INCS = -I${X11INC} -I${FREETYPEINC} 
LIBS = -L${X11LIB} -lX11 ${XINERAMALIBS} ${FREETYPELIBS}  -lglog

CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200809L -DVERSION=\"${VERSION}\" ${XINERAMAFLAGS}
CXXFLAGS   = -std=c++17 -pedantic -Wall -Wno-deprecated-declarations -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}



all: tgwm clean

HEADERS = $(shell find ./src -name "*.hpp")
SOURCES = $(shell find ./src -name "*.cpp")

OBJECTS = $(SOURCES:.cpp=.o)

tgwm: $(HEADERS) $(OBJECTS)
	$(CXX) -o out/$@ $(OBJECTS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f tgwm $(OBJECTS)

