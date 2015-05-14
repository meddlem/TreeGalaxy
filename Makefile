CC = g++ -std=c++11 
LIBS = -lGLEW -lGL -lSDL -lGLU -lglfw
DEBUG = -g
CFLAGS = -c $(DEBUG) 
LFLAGS = $(DEBUG)
OPTIMAZATION=  -O3 -fopenmp 
OBJS = tree.o init.o galaxysim.o render.o main.o
PROGRAM_NAME = program

program: $(OBJS)
	$(CC)  $(LFLAGS) $(OBJS)  -o $(PROGRAM_NAME) $(LIBS)
tree.o: tree.cpp headers.h 
	$(CC) $(CFLAGS) tree.cpp 
render.o: render.cpp #Rendering.h
	$(CC) $(CFLAGS) render.cpp 
init.o: init.cpp headers.h
	$(CC) $(CFLAGS) init.cpp 
galaxysim.o: headers.h galaxysim.cpp
	$(CC) $(CFLAGS) galaxysim.cpp
main.o: main.cpp headers.h tree.cpp init.cpp galaxysim.cpp render.cpp
	$(CC) $(CFLAGS) main.cpp

run :  $(PROGRAM_NAME)
	./$(PROGRAM_NAME)


.PHONY: clean
clean:
		$(RM) $(PROGRAM_NAME) $(OBJS) *.mod
		$(RM) plot*.png output.txt
