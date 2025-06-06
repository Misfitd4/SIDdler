CC=clang
CFLAGS=-Wall -O2 `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs`
OBJS=main.o render.o sid.o

siddler: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

main.o: main.c render.h sid.h
	$(CC) $(CFLAGS) -c main.c

render.o: render.c render.h
	$(CC) $(CFLAGS) -c render.c

sid.o: sid.c sid.h render.h
	$(CC) $(CFLAGS) -c sid.c

clean:
	rm -f *.o siddler
