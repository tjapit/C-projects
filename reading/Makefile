CC = gcc
CFLAGS = -Wall -std=c99 -g
OBJS = input.o catalog.o reading.o

default: clean reading

reading: $(OBJS)
	$(CC) $(OBJS) -o reading
	
reading.o: reading.c catalog.o input.o
	$(CC) $(CFLAGS) -c reading.c

catalog.o: input.o catalog.c catalog.h
	$(CC) $(CFLAGS) -c catalog.c
	
input.o: input.c input.h
	$(CC) $(CFLAGS) -c input.c
	
clean:
	rm -f *.o
	rm -f reading
	rm -f *.exe
	rm -f *.exe.stackdump
	rm -f actual-*.txt
	rm -f output.txt
	rm -f stderr*.txt
	rm -f stdout*.txt
	