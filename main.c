#include <stdio.h>
#include "tabuleiro.h"
#include "jogadores.h"

int main() {
    tabuleiro_iniciar();

    printf("Pressione ENTER para começar...\n");
    getchar();

    tabuleiro_desenhar();
    jogadores_iniciar();
    jogadores_esperar_fim();

    return 0;
}
