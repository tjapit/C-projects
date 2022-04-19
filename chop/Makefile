OBJS = text.o edit.o chop.o
CC = gcc
CFLAGS = -Wall -std=c99 -g

default: chop

clean:
	rm -f *.o
	rm -f chop	# For testing in the eos servers	
	rm -f *.exe
	rm -f *.exe.stackdump
	rm -f actual-*.txt
	rm -f output.txt
	rm -f stderr*.txt
	rm -f stdout*.txt
	
chop: $(OBJS)
	$(CC) $(OBJS) -o chop
	
chop.o: chop.c
	$(CC) $(CFLAGS) -c chop.c -o chop.o

text.o: text.c text.h
	$(CC) $(CFLAGS) -c text.c -o text.o

edit.o: edit.c edit.h
	$(CC) $(CFLAGS) -c edit.c -o edit.o
	
test:
	./chop input-a.txt actual-01.txt
	diff expected-01.txt actual-01.txt
	./chop line 3 input-a.txt actual-02.txt
	diff expected-02.txt actual-02.txt
	./chop lines 2 25 input-a.txt actual-03.txt
	diff expected-03.txt actual-03.txt
	
#	Tests failed

#	Tests passed
#	./chop cols 1 line 1 input-f.txt output.txt > stdout.txt 2> stderr.txt
#	diff estderr-18.txt stderr.txt
#	./chop cols 5 10 lines 10 15 - output.txt < input-a.txt > stdout.txt 2> stderr.txt	# Test 15
#	diff expected-15.txt output.txt
#	./chop cols 5 10 lines 10 15 input-a.txt - > stdout.txt 2> stderr.txt	# Test 16
#	diff estdout-16.txt stdout.txt
#	./chop cols 16 35 input-a.txt output.txt > stdout.txt 2> stderr.txt	# Test 13
#	diff expected-13.txt output.txt
#	./chop lines 5 seven input-a.txt output.txt > stdout.txt 2> stderr.txt	# Test 22
#	diff expected-22.txt output.txt
	
#	In-house testing
#	./chop input-b.txt actual-b.txt
#	diff expected-b.txt actual-b.txt
#	./chop input-c.txt actual-c.txt
#	diff expected-c.txt actual-c.txt
#	./chop input-d.txt actual-d.txt
#	diff expected-d.txt actual-d.txt
#	./chop input-e.txt actual-e.txt > stdout.txt 2> stderr-e.txt
#	./chop input-f.txt actual-f.txt > stdout.txt 2> stderr-f.txt
	