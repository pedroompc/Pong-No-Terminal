#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "../cli-lib/include/screen.h"
#include "../cli-lib/include/keyboard.h"
#include "../cli-lib/include/timer.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define WINNING_SCORE 10

typedef enum {
    MENU,
    PLAYING,
    GAME_OVER
} GameStatus;
typedef struct ScoreNode {
    int placar_esquerda;
    int placar_direita;
    struct ScoreNode *next;
} ScoreNode;

typedef struct {
    int raquete_esquerda, raquete_direita;
    int bola_x, bola_y;
    int bola_dir_x, bola_dir_y;
    int placar_esquerda, placar_direita;
    bool quit;
    GameStatus status;
    int jogador_vencedor;
    char **campo;
} GameState;

void jogo_inicio(GameState *game);
void processar_input(GameState *game);
void atualizar_jogo(GameState *game);
void renderizar(GameState *game);
void resetar_bola(GameState *game);
void liberar(GameState *game);

#endif
