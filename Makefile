
CXXFLAGS ?= -Wall -g
CXXFLAGS += -std=c++1y
CXXFLAGS += `pkg-config --cflags x11 libglog`
LDFLAGS += `pkg-config --libs x11 libglog`

all: mywm build-clean

SOURCES = src/wm.h src/util.h src/main.cpp 

OBJECTS = $(SOURCES:.cpp=.o)

mywm: $(OBJECTS)
	$(CXX) -o out/$@ $(OBJECTS) $(LDFLAGS)

build-clean:
	rm -f src/*.o	

.PHONY: clean
clean:
	rm -f basic_wm $(OBJECTS)