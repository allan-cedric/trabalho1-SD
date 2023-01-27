CFLAGS = -Bstatic
LDLIBS = -lm

all: vcube

vcube: vcube.o smpl.o rand.o cisj.o
vcube.o: vcube.c smpl.h

rand.o: rand.c
smpl.o: smpl.c smpl.h
cisj.o: cisj.c cisj.h

clean:
	$(RM) *.o vcube
