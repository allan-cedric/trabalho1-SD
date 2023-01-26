/* Disciplina Sistemas Distribuídos 
   Prof. Elias P. Duarte Jr.
   O primeiro programa de simulação: tempo.c
   Cada processo simplesmente conta o tempo
   Esta atualização: 29/11/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include "smpl.h"

/* cada processo conta seu tempo */

/*---- aqui definimos os eventos ----*/
#define test   1
#define fault  2
#define recovery 3

/*---- declaramos agora o TipoProcesso ----*/
typedef struct {
   int id;       /* identificador de facility SMPL */
   /* variáveis locais do processo são declaradas aqui */
} TipoProcesso;

TipoProcesso *processo; 

int main (int argc, char *argv[]) {
    static int N,      /* number of nodes is parameter */
               token,  /* node identifier, natural number */ 
               event,
               r,
               i;

    static char fa_name[5];

    if (argc != 2) {
      puts("Uso correto: tempo <num-processos>");
      exit(1);
    }

    N = atoi(argv[1]);
    smpl(0, "Um Exemplo de Simulação");
    reset();
    stream(1);
 
/*----- inicializacao -----*/

    processo = (TipoProcesso *) malloc(sizeof(TipoProcesso)*N);

    for (i=0; i<N; i++) {
       memset (fa_name, '\0', 5);
       sprintf(fa_name, "%d", i);
       processo[i].id = facility(fa_name,1);
       // printf("fa_name = %s, processo[%d].id = %d\n", fa_name, i, processo[i].id);
    } /* end for */
        
/*----- vamos escalonar os eventos iniciais -----*/

    for (i=0; i<N; i++) {
       schedule(test, 30.0, i);
     } 
    schedule(fault, 31.0, 1);
    schedule(recovery, 61.0, 1);
    
/*----- agora vem o loop principal do simulador -----*/

    while (time() < 150.0) {
      cause(&event, &token);
      switch(event) {
       case test: 
         if (status(processo[token].id) != 0) break; // processo falho não testa!
         printf("o processo %d vai testar no tempo %5.1f\n", token, time());
         schedule(test, 30.0, token);
         break;
       case fault:
         r = request(processo[token].id, token, 0);
         printf("o processo %d falhou no tempo %5.1f\n", token, time());
         break;
       case recovery:
         release(processo[token].id, token);
         schedule(test, 1.0, token);
         printf("o processo %d recuperou no tempo %5.1f\n", token, time());
         break;
      } /* end switch */
    } /* end while */
} /* end tempo.c */

