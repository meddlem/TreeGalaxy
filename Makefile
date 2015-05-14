CC = g++ -std=c++11 
LIBS = -lGLEW -lGL -lSDL -lGLU -lglfw
DEBUG = -g
CFLAGS = -c $(DEBUG) 
LFLAGS = $(DEBUG)
OPTIMAZATION=  -O3 -fopenmp 
OBJS = tree.o init.o galaxysim.cpp renderingtest.o main.o
PROGRAM_NAME = program

program: $(OBJS)
	$(CC)  $(LFLAGS) $(OBJS)  -o $(PROGRAM_NAME) $(LIBS)
renderingtest.o: Rendering.h renderingtest.cpp
	$(CC) $(CFLAGS) renderingtest.cpp 
init.o: init.cpp tree.h
	$(CC) $(CFLAGS) init.cpp 
tree.o: tree.cpp tree.h 
	$(CC) $(CFLAGS) tree.cpp 
galaxysim.o: tree.h galaxysim.cpp
	$(CC) $(CFLAGS) galaxysim.cpp
main.o: main.cpp tree.h tree.cpp init.cpp galaxysim.cpp
	$(CC) $(CFLAGS) main.cpp

run :  $(PROGRAM_NAME)
	./$(PROGRAM_NAME)


.PHONY: clean
clean:
		$(RM) $(PROGRAM_NAME) $(OBJS) *.mod
		$(RM) plot*.png output.txt
