F90 = gfortran
CPP = g++
CPPFLAGS = -std=c++11
CPPLIBS = -lgfortran -lGLEW -lGL -lSDL -lGLU -lglfw 
F90FLAGS = -ffast-math -Wall -march=native -O3 

PROG = treegalaxy

#required objects: 
OBJS =
OBJS += constants.o
OBJS += treestructs.o
OBJS += tree.o
OBJS += initialize.o
OBJS += interactions.o
OBJS += render.o
OBJS += treegalaxy.o

all: $(PROG)

treegalaxy: $(OBJS) 
	$(CPP) -o $@ $^ $(CPPLIBS)

%.o: %.f95
	$(F90) $(F90FLAGS) -o $@ -c $<

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(CPPLIBS) -o $@ -c $<

.PHONY: clean
clean:
	$(RM) $(PROG) $(OBJS) *.mod *.o
	$(RM) plot*.png output.txt *.out
