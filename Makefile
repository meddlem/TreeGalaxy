FC = gfortran
FFLAGS = -ffast-math -Wall -march=native -O3 -fopenmp #compiler flags
LDFLAGS = -fopenmp #link flags

FFLAGS += $(shell pkg-config --cflags plplotd-f95)
LIBS += $(shell pkg-config --libs plplotd-f95)

COMPILE = $(FC) $(FFLAGS)
LINK = $(FC) $(LDFLAGS)

PROG = main #program name

#required objects: 
OBJS =
OBJS += constants.o
OBJS += treestructs.o
OBJS += tree.o
OBJS += initialize.o
OBJS += interactions.o
OBJS += plotroutines.o
OBJS += main.o

all: $(PROG)

main: $(OBJS)
	$(LINK) -o $@ $^ $(LIBS)

%.o: %.f95
	$(COMPILE) -o $@ -c $<

.PHONY: clean
clean:
	$(RM) $(PROG) $(OBJS) *.mod
	$(RM) plot*.png output.txt *.out
