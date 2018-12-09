WRKDIR = `pwd`
CC = gcc
CPP = g++
F77 = f77
F9X = gfortran
LD = g++
AR = ar
RANLIB = ranlib
WINDRES = windres

INC = -Iglut/include
CFLAGS = -Wall -std=c++11
RESINC =
RCFLAGS =
LIBDIR = -Lglut/lib -L/usr/local/cuda/lib
LIB = -lglut -lGLU -lGL -lm -lcuda -lcudart -lpthread
LDFLAGS =

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O3
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj
DEP_RELEASE =
OUT_RELEASE = ../bin/babouche

OBJ_RELEASE = $(OBJDIR_RELEASE)/Matrices.o $(OBJDIR_RELEASE)/main.o

all: release

clean: clean_release

release: $(OUT_RELEASE)

$(OUT_RELEASE): $(OBJ_RELEASE) $(DEP_RELEASE)
	test -d ../bin || mkdir -p ../bin
	$(LD) $(LDFLAGS_RELEASE) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE) $(OBJDIR_RELEASE)/algo.o $(LIB_RELEASE)

$(OBJDIR_RELEASE)/Matrices.o: Matrices.cpp
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)
	$(CPP) $(CFLAGS_RELEASE) $(INC_RELEASE) -c -o $(OBJDIR_RELEASE)/Matrices.o Matrices.cpp

$(OBJDIR_RELEASE)/main.o: main.cpp
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)
	$(CPP) $(CFLAGS_RELEASE) $(INC_RELEASE) -c -o $(OBJDIR_RELEASE)/main.o main.cpp


clean_release:
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)

.PHONY: clean clean_release
