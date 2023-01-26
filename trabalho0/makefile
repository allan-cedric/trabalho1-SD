CFLAGS = -Bstatic
LDLIBS = -lm

all: tempo tarefa1

tempo: tempo.o smpl.o rand.o
tempo.o: tempo.c smpl.h

tarefa1: tarefa1.o smpl.o rand.o
tarefa1.o: tarefa1.c smpl.h

rand.o: rand.c
smpl.o: smpl.c smpl.h

clean:
	$(RM) *.o tempo tarefa?
