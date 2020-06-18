PROJECT := $(shell pwd)
SRCDIR  := $(PROJECT)/..
SRC := $(wildcard $(SRCDIR)/src/*.cpp)
INCLUDE := -I $(SRCDIR)/src/
CXX := g++
CFLAGS := -std=c++11 -g -pg -Wall -O3 -D_PTHREADS
LIBS := -lpthread

MAIN := $(wildcard $(PROJECT)/main.cpp)
override MAIN_SRC := $(SRC) $(MAIN) 
MAIN_TARGET := main

TEST := $(wildcard $(PROJECT)/test.cpp)
override TEST_SRC := $(SRC) $(TEST)
TEST_TARGET := test


$(MAIN_TARGET): $(MAIN_SRC)
	$(CXX) $(CFLAGS) $(INCLUDE) -o $@ $^ $(LIBS)

$(TEST_TARGET): $(TEST_SRC)
	$(CXX) $(CFLAGS) $(INCLUDE) -o $@ $^ $(LIBS)

clean :
	find . -name '*.o' | xargs rm -f
	find . -name $(MAIN_TARGET) | xargs rm -f
	find . -name $(TEST_TARGET) | xargs rm -f