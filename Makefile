CC =cc
CFlags =-c -Wall -g

all: first

first: test.o func.o 
	$(CC) test.o func.o -o project -lpthread
	
test.o: test.c
	$(CC) $(CFlags) test.c
	
func.o: func.c
	$(CC) $(CFlags) func.c
	
#Deleting all object files
clean:
	rm -rf *o project
