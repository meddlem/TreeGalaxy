CC = g++ 

LIBS = 

DEBUG = -g

CFLAGS = -c $(DEBUG) 

LFLAGS = $(DEBUG)

OPTIMAZATION=  -O3 -fopenmp 

OBJS = galaxysim.o tree.o init.o

PROGRAM_NAME = program

program: $(OBJS)
	$(CC)  $(LFLAGS) $(OBJS)  -o $(PROGRAM_NAME) $(LIBS)
init.o: init.cpp tree.h
	$(CC) $(CFLAGS) init.cpp 
tree.o: tree.cpp tree.h init.cpp
	$(CC) $(CFLAGS) tree.cpp 
galaxysim.o: tree.cpp tree.h init.cpp galaxysim.cpp
	$(CC) $(CFLAGS) galaxysim.cpp

run :  $(PROGRAM_NAME)
	./$(PROGRAM_NAME)


.PHONY: clean
clean:
		$(RM) $(PROGRAM_NAME) $(OBJS) *.mod
		$(RM) plot*.png output.txt
