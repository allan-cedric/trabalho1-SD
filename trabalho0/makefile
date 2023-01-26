CFLAGS = -Bstatic
LDLIBS = -lm

all: tempo tarefa1 tarefa2 tarefa3

tempo: tempo.o smpl.o rand.o
tempo.o: tempo.c smpl.h

tarefa1: tarefa1.o smpl.o rand.o
tarefa1.o: tarefa1.c smpl.h

tarefa2: tarefa2.o smpl.o rand.o
tarefa2.o: tarefa2.c smpl.h

tarefa3: tarefa3.o smpl.o rand.o
tarefa3.o: tarefa3.c smpl.h

rand.o: rand.c
smpl.o: smpl.c smpl.h

clean:
	$(RM) *.o tempo tarefa?
