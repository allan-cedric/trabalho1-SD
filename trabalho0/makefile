CFLAGS = -Bstatic
LDLIBS = -lm

all: tempo

tempo: tempo.o smpl.o rand.o

tempo.o: tempo.c smpl.h

rand.o: rand.c

smpl.o: smpl.c smpl.h

clean:
	$(RM) *.o tempo
