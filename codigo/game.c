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
