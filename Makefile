CC=g++
NP_INCLUDE=`python -c "import numpy; print numpy.get_include()"`
CFLAGS=-I/usr/include/python2.7 -I/usr/include/eigen3 -I./libs/_install/include -I$(HOME)/tm/v2/libs/_install/include -I$(NP_INCLUDE) -fpic -Wall -std=c++11
LDFLAGS=-L./libs/_install/lib -L$(HOME)/slash/lib -lboost_numpy -lboost_python -lglog
SOURCES=hello.cpp hello_mod.cpp
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
