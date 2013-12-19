CC = g++
RM = rm -f
MKDIR = mkdir -p
INSTALL = install

PREFIX ?= /usr

CFLAGS = $(shell pkg-config --cflags eigen3 python) -I$(NP_INCLUDE) -fpic -Wall -std=c++0x
LDFLAGS = $(shell pkg-config --libs eigen3 python) -lboost_numpy -lboost_python
NP_INCLUDE = $(shell python -c "import numpy; print numpy.get_include()")

HEADERS = src/eigen_numpy.h
SOURCES = src/eigen_numpy.cc
LIBRARY = lib/libboost_numpy_eigen.so
OBJECTS = $(SOURCES:.cc=.o)

TEST_SOURCES=src/test_eigen_numpy_mod.cc
TEST_LIBRARY=lib/test_eigen_numpy_mod.so
TEST_OBJECTS=$(TEST_SOURCES:.cc=.o)

.PHONY: all install clean test

all: $(SOURCES) $(LIBRARY)

install: $(HEADERS) $(LIBRARY)
	$(MKDIR) '$(PREFIX)/include/boost/numpy' '$(PREFIX)/lib'
	$(INSTALL) -m644 $(HEADERS) '$(PREFIX)/include/boost/numpy'
	$(INSTALL) -m644 $(LIBRARY) '$(PREFIX)/lib'

clean:
	$(RM) $(LIBRARY) $(OBJECTS) $(TEST_OBJECTS)

test: $(TEST_SOURCES) $(TEST_LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(CC) -shared -Wl,-soname,'$(notdir $@)' $(OBJECTS) $(LDFLAGS) -fpic -o $@

$(TEST_LIBRARY): $(TEST_OBJECTS)
	$(CC) -shared -Wl,-soname,'$(notdir $@)' $(TEST_OBJECTS) $(LDFLAGS) -fpic -o $@

%.o: %.cc
	$(CC) -c $(CFLAGS) '$<' -o '$@'
