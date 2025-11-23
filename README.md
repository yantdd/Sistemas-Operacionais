# Campo Minado com Threads e Semáforos

Este projeto implementa uma versão do jogo Campo Minado em linguagem C. O diferencial desta implementação é que o jogador humano compete contra um Bot em tempo real (baseado em turnos), utilizando conceitos de Sistemas Operacionais para gerenciar a execução simultânea.

O projeto foi desenvolvido como Trabalho Prático para a disciplina **SSC0140 - Sistemas Operacionais I** (ICMC/USP).

## Objetivo
O objetivo do jogo é revelar casas no tabuleiro sem atingir uma mina. 
- Se revelar uma casa vazia ou com número, você ganha pontos.
- Se revelar uma mina, você perde (explode).
- O jogo termina quando alguém explode ou todas as casas seguras são reveladas.

## Conceitos

O software utiliza primitivas de sincronização da biblioteca `pthread` para gerenciar a concorrência entre o jogador e o Bot:

* **Threads (`pthread_t`):** Duas threads separadas são criadas, uma para controlar o fluxo do Humano e outra para o Bot.
* **Semáforos (`sem_t`):** Utilizados para coordenar os turnos. [cite_start]Os semáforos garantem a exclusão mútua no acesso ao tabuleiro (região crítica) e impõem a ordem de jogada (Humano -> Bot -> Humano...).

## Pré-requisitos

* GCC 
* Make (opcional, mas recomendado)
* Ambiente Linux ou compatível (WSL, macOS) devido ao uso de `<pthread.h>` e `<semaphore.h>`.

## Como Compilar e Rodar o Jogo

O projeto já inclui um `Makefile` para facilitar a compilação. No terminal, execute:

```bash
make
make run
```
