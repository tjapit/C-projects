CC = gcc
CFLAGS = -Wall -std=c99 -g

#all: clean brighten border blur test

brighten: brighten.o image.o image.h
	$(CC) brighten.o image.o -o brighten
brighten.o: brighten.c
	$(CC) $(CFLAGS) -c brighten.c -o brighten.o
	
border: border.o image.o image.h
	$(CC) border.o image.o -o border
border.o: border.c
	$(CC) $(CFLAGS) -c border.c -o border.o

blur: blur.o image.o image.h
	$(CC) blur.o image.o -o blur
blur.o: blur.c
	$(CC) $(CFLAGS) -c blur.c -o blur.o
	
image.o: image.c image.h
	$(CC) $(CFLAGS) -c image.c -o image.o
	
clean:
	rm -f *.o
	rm -f *.exe
	rm -f actual-*.ppm
	
#test:
##	brighten
#	./brighten < image-1.ppm > actual-01.ppm;
#	diff expected-01.ppm actual-01.ppm;
#	./brighten < image-2.ppm > actual-02.ppm;
#	diff expected-02.ppm actual-02.ppm;
##	border
#	./border < image-3.ppm > actual-05.ppm;
#	diff expected-05.ppm actual-05.ppm;	
#	./border < image-4.ppm > actual-06.ppm;
#	diff expected-06.ppm actual-06.ppm;
##	blur
##	./blur < image-4.ppm > actual-09.ppm;
##	diff expected-09.ppm actual-09.ppm;
#	./blur < image-5.ppm > actual-10.ppm;
#	diff expected-10.ppm actual-10.ppm;