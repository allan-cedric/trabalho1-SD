/*
  Trabalho pratico 0 da disciplina de Sistemas Distribuidos (CI1088)

  Tarefa 4: Quando um processo correto testa outro processo correto obtém as informações do estado
            dos demais processos do sistema, processos do sistema exceto aqueles que testou nesta rodada,
            além do próprio testador.

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
  int id;          /* identificador de facility SMPL */
  int *State;      /* estado dos processos */
  int *Tested;     /* processos foram testados */
  int rodada_flag; /* verifica se já cumpriu a rodada */
                   /* vari�veis locais do processo s�o declaradas aqui */
} TipoProcesso;

// Retorna 1 se esta falho, senao 0
int processo_falho(TipoProcesso p);

// Retorna 1 se a rodada esta completa, senao 0
int rodada_completa(TipoProcesso *ps, int N);

TipoProcesso *processo;

int main(int argc, char *argv[])
{
  static int N, /* number of nodes is parameter */
      token,    /* node identifier, natural number */
      prox_tk,
      ini_rodada,
      cont_rodada,
      event,
      r,
      i, j;

  static char fa_name[5];

  if (argc != 2)
  {
    puts("Uso correto: tarefa4 <num-processos>");
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
         "Tarefa 4: Quando um processo correto testa outro processo correto obtém as informações do estado\n"
         "dos demais processos do sistema, processos do sistema exceto aqueles que testou nesta rodada,\n"
         "além do próprio testador.\n\n"
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
    for (int j = 0; j < N; j++)
      processo[i].State[j] = unknown;
    processo[i].State[i] = correto;

    processo[i].Tested = (int *)calloc(N, sizeof(int));
    processo[i].rodada_flag = 0;
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

  ini_rodada = 1;
  cont_rodada = 1;
  while (time() < 150.0)
  {
    cause(&event, &token);
    switch (event)
    {
    case test:

      // inicio de uma rodada
      if (ini_rodada)
      {
        printf("\n**************** RODADA %i ****************\n\n", cont_rodada);
        ini_rodada = 0;
      }

      if (processo_falho(processo[token])) // processo falho n�o testa!
        break;

      prox_tk = (token + 1) % N;

      // enquanto existe um processo diferente e que esta falho
      while (prox_tk != token && processo_falho(processo[prox_tk]))
      {
        processo[token].State[prox_tk] = falho;
        processo[token].Tested[prox_tk] = 1;
        printf("o processo %i testou o processo %i FALHO no tempo %5.1f\n", token, prox_tk, time());
        prox_tk = (prox_tk + 1) % N;
      }

      // tem um processo diferente nao falho
      if (prox_tk != token)
      {
        for (i = 0; i < N; i++)
        {
          if (i != token && !processo[token].Tested[i])
            processo[token].State[i] = processo[prox_tk].State[i];
        }
        processo[token].Tested[prox_tk] = 1;
        processo[token].rodada_flag = 1;
        printf("o processo %i testou o processo %i CORRETO no tempo %5.1f\n", token, prox_tk, time());
      }
      else // todo resto dos processos estao falhos
        processo[token].rodada_flag = 1;

      // vetor State
      printf("State_%i: {%i", token, processo[token].State[0]);
      for (i = 1; i < N; i++)
        printf(", %i", processo[token].State[i]);
      printf("}\n\n");

      // fim de rodada
      if (rodada_completa(processo, N))
      {
        printf("**************** FIM DA RODADA %i ****************\n\n\n", cont_rodada);
        for (i = 0; i < N; i++)
        {
          if (!processo_falho(processo[i]))
          {
            memset(processo[i].Tested, 0, sizeof(int) * N);
            processo[i].rodada_flag = 0;
          }
        }
        cont_rodada++;
        ini_rodada = 1;
      }

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
  {
    free(processo[i].State);
    free(processo[i].Tested);
  }
  free(processo);

  return 0;
} /* end tempo.c */

int processo_falho(TipoProcesso p)
{
  return status(p.id) != 0;
}

int rodada_completa(TipoProcesso *ps, int N)
{
  for(int i = 0; i < N; i++)
  {
    if(!processo_falho(ps[i]) && !ps[i].rodada_flag)
      return 0;
  }
  return 1;
}
