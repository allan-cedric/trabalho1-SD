/*
  Trabalho pratico 0 da disciplina de Sistemas Distribuidos (CI1088)

  Tarefa 1: Fazer cada um dos processos testar o seguinte no anel

  Autor: Allan Cedric G. B. Alves da Silva - GRR20190351
*/

#include <stdio.h>
#include <stdlib.h>
#include "smpl.h"

/* cada processo conta seu tempo */

/*---- aqui definimos os eventos ----*/
#define test 1
#define fault 2
#define recovery 3

/*---- declaramos agora o TipoProcesso ----*/
typedef struct
{
  int id; /* identificador de facility SMPL */
          /* vari�veis locais do processo s�o declaradas aqui */
} TipoProcesso;

TipoProcesso *processo;

int main(int argc, char *argv[])
{
  static int N, /* number of nodes is parameter */
      token,    /* node identifier, natural number */
      prox_tk,  /* next token to test */
      event,
      r,
      i;

  static char fa_name[5];

  if (argc != 2)
  {
    puts("Uso correto: tarefa1 <num-processos>");
    exit(1);
  }

  if (atoi(argv[1]) < 2)
  {
    puts("Erro: O sistema precisa de no minimo 2 processos");
    exit(1);
  }

  N = atoi(argv[1]);
  smpl(0, "Um Exemplo de Simula��o");
  reset();
  stream(1);

  printf("Trabalho pratico 0 da disciplina de Sistemas Distribuidos (CI1088)\n\n"
         "Tarefa 1: Fazer cada um dos processos testar o seguinte no anel\n\n"
         "Autor: Allan Cedric G. B. Alves da Silva - GRR20190351\n\n");

  printf("Info.: N = %i\n\n", N);

  /*----- inicializacao -----*/

  processo = (TipoProcesso *)malloc(sizeof(TipoProcesso) * N);

  for (i = 0; i < N; i++)
  {
    memset(fa_name, '\0', 5);
    sprintf(fa_name, "%d", i);
    processo[i].id = facility(fa_name, 1);
    // printf("fa_name = %s, processo[%d].id = %d\n", fa_name, i, processo[i].id);
  } /* end for */

  /*----- vamos escalonar os eventos iniciais -----*/

  for (i = 0; i < N; i++)
  {
    schedule(test, 30.0, i);
  }
  schedule(fault, 31.0, 1);
  schedule(recovery, 61.0, 1);

  /*----- agora vem o loop principal do simulador -----*/

  while (time() < 150.0)
  {
    cause(&event, &token);
    switch (event)
    {
      case test:
        if (status(processo[token].id) != 0) // processo falho n�o testa!
          break;

        prox_tk = (token + 1) % N;

        if(status(processo[prox_tk].id) != 0) // processo esta falho
          printf("o processo %i testou o processo %i FALHO no tempo %5.1f\n", token, prox_tk, time());
        else
          printf("o processo %i testou o processo %i CORRETO no tempo %5.1f\n", token, prox_tk, time());
        
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

  free(processo);

  return 0;
} /* end tempo.c */
