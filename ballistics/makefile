CC = gcc
CFLAGS = -Wall -std=c99 -g

all: clean compile check test 

clean: 
	rm style textbox ballistics check
	
compile:
	$(CC) $(CFLAGS) style.c -o style;
	$(CC) $(CFLAGS) textbox.c -o textbox;
	$(CC) $(CFLAGS) ballistics.c -o ballistics;
	
	
test:
#	# style
#	./style; 
	
	# textbox
	./textbox < input-t1.txt > actual-t1.txt;	#t1
	diff expected-t1.txt actual-t1.txt;
	./textbox < input-t2.txt > actual-t2.txt;	#t2
	diff expected-t2.txt actual-t2.txt;
	./textbox < input-t3.txt > actual-t3.txt;	#t3
	diff expected-t3.txt actual-t3.txt;
	./textbox < input-t4.txt > actual-t4.txt;	#t4
	diff expected-t4.txt actual-t4.txt; 
	
	#ballistics
	./ballistics < input-b1.txt > actual-b1.txt;	#b1
	diff expected-b1.txt actual-b1.txt; exit 0;
	./ballistics < input-b2.txt > actual-b2.txt;	#b2
	diff expected-b2.txt actual-b2.txt; exit 0;
	./ballistics < input-b3.txt > actual-b3.txt;	#b3
	diff expected-b3.txt actual-b3.txt; exit 0;
	./ballistics < input-b4.txt > actual-b4.txt;	#b4
	diff expected-b4.txt actual-b4.txt; exit 0;
	
check:
	$(CC) $(CFLAGS) check.c -o check;
	./check < style.c;
	./check < textbox.c;
	./check < ballistics.c;
	