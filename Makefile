CC=g++
NP_INCLUDE=`python -c "import numpy; print numpy.get_include()"`
CFLAGS=-I/usr/include/python2.7 -I/usr/include/eigen3 -I./libs/_install/include -I$(HOME)/tm/v2/libs/_install/include -I$(NP_INCLUDE) -fpic -Wall -std=c++11
LDFLAGS=-L./libs/_install/lib -L$(HOME)/slash/lib -lboost_numpy -lboost_python -lglog
SOURCES=hello.cc hello_mod.cc eigen_numpy.cc
OBJECTS=$(SOURCES:.cc=.o)
LIBRARY=libhello.so

TEST_SOURCES=test_eigen_numpy_mod.cc eigen_numpy.cc
TEST_LIBRARY=test_eigen_numpy_mod.so
TEST_OBJECTS=$(TEST_SOURCES:.cc=.o)

all: $(SOURCES) $(LIBRARY)

test: $(TEST_SOURCES) $(TEST_LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(CC) -shared -Wl,-soname,"$@" $(OBJECTS) $(LDFLAGS) -fpic -o $@

$(TEST_LIBRARY): $(TEST_OBJECTS)
	$(CC) -shared -Wl,-soname,"$@" $(TEST_OBJECTS) $(LDFLAGS) -fpic -o $@

.cc.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f libhello.so
	rm -f test_eigen_numpy_mod.so
