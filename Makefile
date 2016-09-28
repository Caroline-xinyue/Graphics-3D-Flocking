# basic Makefile 
# compile with "make <filename without .c>
# Note that if you installed your OpenGL libraries in a different place
# the paths that follow -L in LDFLAGS and -I in CFLAGS will have to change
# accordingly

ifeq ($(shell uname -s), Darwin)
LDFLAGS = -L/opt/ports/lib -framework OpenGL -lglfw -lGLEW -lm
else
LDFLAGS = -lX11 -lGL -lGLU -lglfw -lGLEW -lm
endif
CC = gcc
CFLAGS = -g -Wall -I/opt/ports/include
DEPS = ll.h hw3.h
OBJ = ll.o hw3.o
OBJ_DEBUG = ll.o hw3_debug.o

all: hw3 hw3_debug
ll.o: ll.c $(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<
hw3.o: hw3.c $(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<
hw3: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
hw3_debug.o: hw3.c $(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -c -DDEBUG -o $@ $<
hw3_debug: $(OBJ_DEBUG)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
clean:
	rm *.o hw3 hw3_debug
