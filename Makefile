# Makefile for Laplace project

# CIS 455/555 Winter 2012

CXX = g++
CXXFLAGS += -fopenmp

LIBS =

laplace:	laplace.o $(OBJS)
		$(LINK.cc) -o ./bin/laplace laplace.o $(LIBS)

laplace.o:	laplace.cpp 

clean:
	rm -rf core *.o *~
