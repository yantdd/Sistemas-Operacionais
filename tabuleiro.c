#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tabuleiro.h"

static int tab[TAB_LINHAS][TAB_COLUNAS]; // Matriz com os valores (-1 mina, 0 vazio, N vizinhos)
static int revelado[TAB_LINHAS][TAB_COLUNAS]; // Mascara de visibilidade (0 oculto, 1 revelado)
static int restantes = TAB_LINHAS * TAB_COLUNAS - TAB_MINAS;

static int dentro(int l, int c) {
    return l >= 0 && l < TAB_LINHAS && c >= 0 && c < TAB_COLUNAS;
}

static int contar_vizinhos(int l, int c) {
    int n = 0;
    for (int i = -1; i <= 1; ++i)
        for (int j = -1; j <= 1; ++j)
            if (i || j)
                if (dentro(l+i, c+j) && tab[l+i][c+j] == -1)
                    n++;
    return n;
}

// Para abrir áreas vazias (zeros) adjacentes
static void expandir(int l, int c) {
    if (!dentro(l,c)) return;
    if (revelado[l][c]) return;

    revelado[l][c] = 1;
    restantes--;

    if (tab[l][c] == 0) {
        for (int i=-1;i<=1;i++)
            for (int j=-1;j<=1;j++)
                if (i||j) expandir(l+i,c+j);
    }
}

// Prepara o tabuleiro e distribui minas
void tabuleiro_iniciar() {
    srand(time(NULL));

    // Zera tudo
    for (int i=0;i<TAB_LINHAS;i++)
        for (int j=0;j<TAB_COLUNAS;j++)
            tab[i][j]=0, revelado[i][j]=0;

    int colocadas = 0;

    // Coloca as minas aleatoriamente
    while (colocadas < TAB_MINAS) {
        int l = rand() % TAB_LINHAS;
        int c = rand() % TAB_COLUNAS;
        if (tab[l][c] != -1) {
            tab[l][c] = -1;
            colocadas++;
        }
    }

    for (int l=0;l<TAB_LINHAS;l++)
        for (int c=0;c<TAB_COLUNAS;c++)
            if (tab[l][c] != -1)
                tab[l][c] = contar_vizinhos(l,c);
}

void tabuleiro_desenhar() {
    printf("     ");
    for (int j=0;j<TAB_COLUNAS;j++) printf("%d  ", j);
    printf("\n   +");
    for (int j=0;j<TAB_COLUNAS*3;j++) printf("-");
    printf("+\n");

    for (int i=0;i<TAB_LINHAS;i++) {
        printf("%2d | ", i);
        for (int j=0;j<TAB_COLUNAS;j++) {
            if (!revelado[i][j]) printf("#  ");
            else if (tab[i][j] == -1) printf("@  ");
            else if (tab[i][j] == 0) printf("   ");
            else printf("%d  ", tab[i][j]);
        }
        printf("|\n");
    }

    printf("   +");
    for (int j=0;j<TAB_COLUNAS*3;j++) printf("-");
    printf("+\n");

}

int tabuleiro_revelar(int l, int c) {
    if (!dentro(l,c)){ 
        printf("Fora dos limites do tabuleiro!\n");
        return TABULEIRO_FORA;}
    if (revelado[l][c]) return TABULEIRO_JA_REVELADO;
    if (tab[l][c] == -1) {
        revelado[l][c] = 1;
        return TABULEIRO_EXPLODIU;
    }

    expandir(l,c);
    return 1;
}

int tabuleiro_restantes() {
    return restantes;
}

int tabuleiro_ja_revelado(int l, int c) {
    if (!dentro(l,c)) return 1;
    return revelado[l][c];
}