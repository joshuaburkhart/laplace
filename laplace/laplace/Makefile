# Makefile for Laplace project

# CIS 455/555 Winter 2012

CXX = g++
CXXFLAGS += -fopenmp

LIBS =
ARRAY = Array.o
OPTIONS = OptionList.o

OBJS = $(ARRAY) $(OPTIONS)

laplace:	laplace.o $(OBJS)
		$(LINK.cc) -o laplace laplace.o $(OBJS) $(LIBS)

laplace.o:	laplace.C Array.h OptionList.h 
Array.o:	Array.C Array.h
OptionList.o:	OptionList.C OptionList.h

clean:
	rm -rf core *.o *~
