#ifndef TABULEIRO_H
#define TABULEIRO_H

// Configuraçoes do jogo
#define TAB_LINHAS 9
#define TAB_COLUNAS 9
#define TAB_MINAS 10

// Codigos de retorno
#define TABULEIRO_EXPLODIU -3
#define TABULEIRO_JA_REVELADO -1
#define TABULEIRO_FORA -2

void tabuleiro_iniciar();
void tabuleiro_desenhar();
int tabuleiro_revelar(int l, int c);
int tabuleiro_restantes();
int tabuleiro_ja_revelado(int l, int c);

#endif
