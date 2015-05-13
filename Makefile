CC = g++ -std=c++11
DEBUG = -g
CFLAGS = -c $(DEBUG) 
LFLAGS = $(DEBUG)
OPTIMAZATION=  -O3 -fopenmp 
OBJS = tree.o galaxysim.o

PROGRAM_NAME = program

program: $(OBJS)
	$(CC)  $(LFLAGS) $(OBJS) -o $(PROGRAM_NAME)

tree.o: tree.h
	$(CC) $(CFLAGS) tree.cpp
galaxysim.o: tree.o tree.h
	$(CC) $(CFLAGS) galaxysim.cpp

clean:
	\rm *.o $(PROGRAM_NAME)
