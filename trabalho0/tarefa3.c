/*
  Trabalho pratico 0 da disciplina de Sistemas Distribuidos (CI1088)

  Tarefa 3: Cada processo mantém localmente o vetor State[N].
            A entrada do vetor State[j] indica o estado do processo j.
            Nesta tarefa ao executar um teste em um processo j,
            o testador atualiza a entrada correspondente no vetor State[j].

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

// estados dos processos
#define unknown -1
#define correto 0
#define falho 1

/*---- declaramos agora o TipoProcesso ----*/
typedef struct
{
  int id;     /* identificador de facility SMPL */
  int *State; /* estado dos processos */
              /* vari�veis locais do processo s�o declaradas aqui */
} TipoProcesso;

// Retorna 1 se esta falho, senao 0
int processo_falho(TipoProcesso p);

TipoProcesso *processo;

int main(int argc, char *argv[])
{
  static int N, /* number of nodes is parameter */
      token,    /* node identifier, natural number */
      prox_tk,
      event,
      r,
      i;

  static char fa_name[5];

  if (argc != 2)
  {
    puts("Uso correto: tarefa3 <num-processos>");
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
         "Tarefa 3: Cada processo mantém localmente o vetor State[N].\n"
         "A entrada do vetor State[j] indica o estado do processo j.\n"
         "Nesta tarefa ao executar um teste em um processo j,\n"
         "o testador atualiza a entrada correspondente no vetor State[j].\n\n"
         "Autor: Allan Cedric G. B. Alves da Silva - GRR20190351\n\n");

  printf("Info.: N = %i\n\n", N);

  /*----- inicializacao -----*/

  processo = (TipoProcesso *)malloc(sizeof(TipoProcesso) * N);

  for (i = 0; i < N; i++)
  {
    memset(fa_name, '\0', 5);
    sprintf(fa_name, "%d", i);
    processo[i].id = facility(fa_name, 1);

    processo[i].State = (int *)malloc(sizeof(int) * N);
    for(int j = 0; j < N; j++)
      processo[i].State[j] = unknown;
    processo[i].State[i] = correto;
    // printf("fa_name = %s, processo[%d].id = %d\n", fa_name, i, processo[i].id);
  } /* end for */

  /*----- vamos escalonar os eventos iniciais -----*/

  for (i = 0; i < N; i++)
  {
    schedule(test, 30.0, i);
  }
  schedule(fault, 31.0, 1);
  schedule(recovery, 61.0, 1);

  for (i = 1; i < N; i++)
    schedule(fault, 63.0, i);

  /*----- agora vem o loop principal do simulador -----*/

  while (time() < 150.0)
  {
    cause(&event, &token);
    switch (event)
    {
      case test:
        if (processo_falho(processo[token])) // processo falho n�o testa!
          break;

        prox_tk = (token + 1) % N;

        // enquanto existe um processo diferente e que esta falho
        while (prox_tk != token && processo_falho(processo[prox_tk]))
        {
          processo[token].State[prox_tk] = falho;
          printf("o processo %i testou o processo %i FALHO no tempo %5.1f\n", token, prox_tk, time());
          prox_tk = (prox_tk + 1) % N;
        }

        // tem um processo diferente nao falho
        if (prox_tk != token)
        {
          processo[token].State[prox_tk] = correto;
          printf("o processo %i testou o processo %i CORRETO no tempo %5.1f\n", token, prox_tk, time());
        }

        // vetor State
        printf("State_%i: {%i", token, processo[token].State[0]);
        for(i = 1; i < N; i++)
          printf(", %i", processo[token].State[i]);
        printf("}\n\n");

        schedule(test, 30.0, token);
        break;
      case fault:
        r = request(processo[token].id, token, 0);
        printf("o processo %d falhou no tempo %5.1f\n\n", token, time());
        break;
      case recovery:
        release(processo[token].id, token);
        schedule(test, 1.0, token);
        printf("o processo %d recuperou no tempo %5.1f\n\n", token, time());
        break;
    } /* end switch */
  }   /* end while */

  for(i = 0; i < N; i++)
    free(processo[i].State);
  free(processo);

  return 0;
} /* end tempo.c */

int processo_falho(TipoProcesso p)
{
  return status(p.id) != 0;
}
