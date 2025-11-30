#include "tabuleiro.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static pthread_t th_humano, th_bot;
static sem_t sem_humano, sem_bot;

static int fim = 0;
static int pontos_humano = 0;
static int pontos_bot = 0;

// Thread que controla as jogadas do humano
static void *thread_humano(void *v) {
  (void)v;
  char buf[32];

  while (!fim) {
    // Espera a vez do humano (semaforo inicia aberto)
    sem_wait(&sem_humano);
    if (fim)
      break;

    printf("SUA VEZ:\nDigite LINHA COLUNA ex: '3 2' ou 's' para sair:\n> ");

    if (!fgets(buf, sizeof(buf), stdin))
      continue;
    if (buf[0] == 's') {
      fim = 1;
      sem_post(&sem_bot); // Libera o bot para ele poder sair do loop tambem
      break;
    }

    int l, c;
    if (sscanf(buf, "%d %d", &l, &c) != 2) {
      printf("Entrada inválida!\n");
      sem_post(&sem_humano); // Devolve o token para o humano tentar de novo
      continue;
    }

    int r = tabuleiro_revelar(l, c);

    if (r == TABULEIRO_EXPLODIU) {
      printf("Você EXPLODIU em (%d,%d)!\n", l, c);
      tabuleiro_desenhar();
      fim = 1;
      sem_post(&sem_bot);
      break;
    }

    if (r > 0)
      pontos_humano += r;

    tabuleiro_desenhar();

    if (tabuleiro_restantes() == 0) {
      fim = 1;
      sem_post(&sem_bot);
      break;
    }

    // Passa a vez para o Bot
    sem_post(&sem_bot);
  }

  return NULL;
}

// Thread que controla as jogadas do computador
static void *thread_bot(void *v) {
  (void)v;
  while (!fim) {

    // Espera a vez do bot
    sem_wait(&sem_bot);
    if (fim)
      break;

    int l, c;

    // Escolhe uma posiçao aleatoria que ainda nao foi revelada
    do {
      l = rand() % TAB_LINHAS;
      c = rand() % TAB_COLUNAS;
    } while (tabuleiro_ja_revelado(l, c));

    printf("VEZ DO BOT:\n");
    int r = tabuleiro_revelar(l, c);

    if (r == TABULEIRO_EXPLODIU) {
      tabuleiro_desenhar();
      printf("Bot abriu (%d,%d) e EXPLODIU!\n", l, c);
      fim = 1;
      sem_post(&sem_humano);
      break;
    }

    if (r > 0) {
      pontos_bot += r;
      tabuleiro_desenhar();
      printf("Bot abriu (%d,%d) e ganhou %d ponto(s). Total=%d\n", l, c, r,
             pontos_bot);
    }

    if (tabuleiro_restantes() == 0) {
      fim = 1;
      sem_post(&sem_humano);
      break;
    }

    // Passa a vez para o Humano
    sem_post(&sem_humano);
  }

  return NULL;
}

// Inicializa semaforos e dispara as threads
void jogadores_iniciar() {
  srand(time(NULL));
  sem_init(&sem_humano, 0, 1); // Humano começa
  sem_init(&sem_bot, 0, 0);    // Bot espera

  // Cria as threads passando NULL onde seria o argumento void*
  pthread_create(&th_humano, NULL, thread_humano, NULL);
  pthread_create(&th_bot, NULL, thread_bot, NULL);
}

// Aguarda o termino das threads e mostra o placar
void jogadores_esperar_fim() {
  pthread_join(th_humano, NULL);
  pthread_join(th_bot, NULL);

  printf("\n=== FIM DE JOGO ===\n");
  printf("Pontos Humano: %d\n", pontos_humano);
  printf("Pontos Bot: %d\n", pontos_bot);

  if (pontos_humano > pontos_bot)
    printf("Você venceu!\n");
  else if (pontos_humano < pontos_bot)
    printf("Bot venceu!\n");
  else
    printf("Empate!\n");
}
