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
        return;
    }

    if (game->status == PLAYING) {
        switch (ch) {
            case 'w':
                if (game->raquete_esquerda > 1) game->raquete_esquerda--;
                break;
            case 's':
                if (game->raquete_esquerda < SCREEN_HEIGHT-2) game->raquete_esquerda++;
                break;
            case 'i':
                if (game->raquete_direita > 1) game->raquete_direita--;
                break;
            case 'k':
                if (game->raquete_direita < SCREEN_HEIGHT-2) game->raquete_direita++;
                break;
        }
    }
}


void atualizar_jogo(GameState *game) {

    if (game->status != PLAYING) return;

    // Mover bola
    game->bola_x += game->bola_dir_x;
    game->bola_y += game->bola_dir_y;

    // --- Ponto para a direita ---
    if (game->bola_x < 0) {
        game->placar_direita++;

        if (game->placar_direita >= WINNING_SCORE) {
            game->status = GAME_OVER;
            game->jogador_vencedor = 2; // Jogador da direita venceu
        } else {
            // Resetar bola
            game->bola_x = SCREEN_WIDTH / 2;
            game->bola_y = SCREEN_HEIGHT / 2;
            game->bola_dir_x = 1;
            game->bola_dir_y = 0;
        }
        return;
    }

    // --- Ponto para a esquerda ---
    if (game->bola_x >= SCREEN_WIDTH) {
        game->placar_esquerda++;

        
        if (game->placar_esquerda >= WINNING_SCORE) {
            game->status = GAME_OVER;
            game->jogador_vencedor = 1; // Jogador da esquerda venceu
        } else {
            // Resetar bola
            game->bola_x = SCREEN_WIDTH / 2;
            game->bola_y = SCREEN_HEIGHT / 2;
            game->bola_dir_x = -1;
            game->bola_dir_y = 0; // 
        }
        return;
    }
    
    // --- Colisão com topo/fundo --- 
    if (game->bola_y <= 0 || game->bola_y >= SCREEN_HEIGHT - 1) {
        game->bola_dir_y *= -1;
        return;
    }

    // --- Colisão Raquete Esquerda --- 
    if (game->bola_x <= 1) {
        if (game->bola_y >= game->raquete_esquerda - 1 &&
            game->bola_y <= game->raquete_esquerda + 1) {
            game->bola_dir_x = 1;
            return;
        }
    }

    // --- Colisão Raquete Direita --- 
    if (game->bola_x >= SCREEN_WIDTH - 2) {
        if (game->bola_y >= game->raquete_direita - 1 &&
            game->bola_y <= game->raquete_direita + 1) {
            game->bola_dir_x = -1;
            return;
        }
    }
}

void renderizar(GameState *game) {
    screenClear();

    // --- desenhar placar  ---
    screenSetColor(FG_WHITE, BG_BLACK);
    screenGotoxy(SCREEN_WIDTH/2 - 3, 0);
    printf("%d - %d", game->placar_esquerda, game->placar_direita);

    // --- desenhar raquete esquerda ---
    screenSetColor(FG_GREEN, BG_BLACK);   
    screenGotoxy(1, game->raquete_esquerda - 1); printf("|");
    screenGotoxy(1, game->raquete_esquerda    ); printf("|");
    screenGotoxy(1, game->raquete_esquerda + 1); printf("|");

    // --- desenhar raquete direita ---
    screenSetColor(FG_GREEN, BG_BLACK);   
    screenGotoxy(SCREEN_WIDTH - 2, game->raquete_direita - 1); printf("|");
    screenGotoxy(SCREEN_WIDTH - 2, game->raquete_direita    ); printf("|");
    screenGotoxy(SCREEN_WIDTH - 2, game->raquete_direita + 1); printf("|");

    // --- desenhar bola ---
    screenSetColor(FG_RED, BG_BLACK);     
    screenGotoxy(game->bola_x, game->bola_y);
    printf("O");

    // restaura cor padrão
    screenSetColor(FG_WHITE, BG_BLACK);

    screenUpdate();
}

