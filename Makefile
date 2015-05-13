CC = g++ -std=c++11

LIBS = 

DEBUG = -g

CFLAGS = -c $(DEBUG) 

LFLAGS = $(DEBUG)

OPTIMAZATION=  -O3 -fopenmp 

OBJS = galaxysim.o tree.o

PROGRAM_NAME = program

program: $(OBJS)
	$(CC)  $(LFLAGS) $(OBJS)  -o $(PROGRAM_NAME) $(LIBS)
tree.o: tree.cpp tree.h 
	$(CC) $(CFLAGS) tree.cpp 
galaxysim.o: tree.cpp tree.h 
	$(CC) $(CFLAGS) galaxysim.cpp

run :  $(PROGRAM_NAME)
	./$(PROGRAM_NAME)


.PHONY: clean
clean:
		$(RM) $(PROGRAM_NAME) $(OBJS) *.mod
		$(RM) plot*.png output.txt
