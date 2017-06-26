INCLUDE = include/
SOURCE = source/
OBJECTS = obj/
BINARIES = bin/

CC = g++
CFLAGS = -Wall -fpermissive -std=c++11

setup:
	mkdir $(OBJECTS) $(BINARIES)

clean:
	rm -f $(OBJECTS)*.o $(BINARIES)*.a exec 
	rm -f -r $(OBJECTS) $(BINARIES)

compile_source: $(SOURCE)grid.cpp $(SOURCE)simulation.cpp clean setup
	$(CC) $(CFLAGS) -I $(INCLUDE) -c $(SOURCE)*.cpp
	mv *.o $(OBJECTS)

build_lib: compile_source
	ar -cvq liblightwave.a $(OBJECTS)*.o
	mv *.a $(BINARIES)

compile_exec:  build_lib
	$(CC) $(CFLAGS) -o exec instantiator.cpp -I $(INCLUDE) -L $(BINARIES) -llightwave 

run: compile_exec
	./exec
