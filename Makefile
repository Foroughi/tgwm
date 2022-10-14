
CXXFLAGS ?= -Wall -g
CXXFLAGS += -std=c++17
CXXFLAGS += `pkg-config --cflags x11 libglog`
LDFLAGS += `pkg-config --libs x11 libglog`

all: mywm build-clean


SOURCES = src/main.cpp

OBJECTS = $(SOURCES:.cpp=.o)

mywm: $(OBJECTS)
	$(CXX) -o out/$@ $(OBJECTS) $(LDFLAGS)



build-clean:
	rm -f src/*.o	

.PHONY: clean
clean:
	rm -f basic_wm $(OBJECTS)