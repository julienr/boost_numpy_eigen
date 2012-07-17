# TODO: Automatically go through all libs in libs and install them (with --prefix)
# to ./libs/_install
CC=g++
CFLAGS=-I/usr/include/python2.7 -I/usr/include/eigen3 -I./libs/_install/include -I$(HOME)/slash/include -fpic -Wall -std=c++0x
LDFLAGS=-L./libs/_install/lib -L$(HOME)/slash/lib -lboost_numpy -lboost_python -lglog
SOURCES=hello.cpp hello_py.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=libhello.so

all: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(CC) -shared -Wl,-soname,"$@" $(OBJECTS) $(LDFLAGS) -fpic -o $@

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f libhello.so
