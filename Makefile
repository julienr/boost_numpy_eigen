CC = g++
RM = rm -f
CFLAGS = $(shell pkg-config --cflags eigen3 python) -I$(NP_INCLUDE) -fpic -Wall -std=c++0x
LDFLAGS = $(shell pkg-config --libs eigen3 python) -lboost_numpy -lboost_python
NP_INCLUDE = $(shell python -c "import numpy; print numpy.get_include()")

SOURCES = src/hello.cc \
          src/hello_mod.cc \
          src/eigen_numpy.cc
LIBRARY = libs/libhello.so
OBJECTS = $(SOURCES:.cc=.o)

TEST_SOURCES=test_eigen_numpy_mod.cc eigen_numpy.cc
TEST_LIBRARY=test_eigen_numpy_mod.so
TEST_OBJECTS=$(TEST_SOURCES:.cc=.o)

all: $(SOURCES) $(LIBRARY)

test: $(TEST_SOURCES) $(TEST_LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(CC) -shared -Wl,-soname,'$@' $(OBJECTS) $(LDFLAGS) -fpic -o $@

$(TEST_LIBRARY): $(TEST_OBJECTS)
	$(CC) -shared -Wl,-soname,'$@' $(TEST_OBJECTS) $(LDFLAGS) -fpic -o $@

.cc.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(LIBRARY) $(OBJECTS) $(TEST_OBJECTS)
