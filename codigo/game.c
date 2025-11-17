#include "game.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void jogo_inicio(GameState *game) {
    srand(time(NULL));

    game->campo = malloc(SCREEN_HEIGHT * sizeof(char *));
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        game->campo[i] = malloc(SCREEN_WIDTH * sizeof(char));
        memset(game->campo[i], ' ', SCREEN_WIDTH);
    }

    game->raquete_esquerda = game->raquete_direita = SCREEN_HEIGHT / 2;
    game->bola_x = SCREEN_WIDTH / 2;
    game->bola_y = SCREEN_HEIGHT / 2;
    game->bola_dir_x = 1;
    game->bola_dir_y = 0;
    game->placar_esquerda = game->placar_direita = 0;
    game->quit = false;
    game->status = PLAYING;
    game->jogador_vencedor = 0;
}

void liberar(GameState *game) {
    if (game->campo) {
        for (int i = 0; i < SCREEN_HEIGHT; i++) {
            free(game->campo[i]);
        }
        free(game->campo);
    }
}

void processar_input(GameState *game) {
    if (!keyhit()) return;

    int ch = readch();
    if (ch == 'q' || ch == 'Q') {
        game->quit = true;
    }
}

void atualizar_jogo(GameState *game) {
    if (game->status != PLAYING) return;

    game->bola_x += game->bola_dir_x;
    game->bola_y += game->bola_dir_y;

    if (game->bola_x < 0) game->bola_x = SCREEN_WIDTH - 1;
    if (game->bola_x >= SCREEN_WIDTH) game->bola_x = 0;
}

void renderizar(GameState *game) {
    screenClear();

    // bola
    screenGotoxy(game->bola_x, game->bola_y);
    putchar('O');

    // raquete esquerda
    for (int i = -1; i <= 1; i++) {
        int e = game->raquete_esquerda + i;
        if (e >= 0 && e < SCREEN_HEIGHT) {
            screenGotoxy(0, e);
            putchar('|');
        }
    }

    // raquete direita
    for (int i = -1; i <= 1; i++) {
        int d = game->raquete_direita + i;
        if (d >= 0 && d < SCREEN_HEIGHT) {
            screenGotoxy(SCREEN_WIDTH-1, d);
            putchar('|');
        }
    }

    screenUpdate();
}


