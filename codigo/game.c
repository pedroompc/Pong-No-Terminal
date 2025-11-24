#include "game.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#define BALL_SPEED 1.8f
#define MAX_SPEED 3.5f


void resetar_bola(GameState *game) {
    game->bola_x = SCREEN_WIDTH / 2;
    game->bola_y = SCREEN_HEIGHT / 2;
    game->bola_dir_x = (rand() % 2) ? BALL_SPEED : -BALL_SPEED;
    game->bola_dir_y = ((rand() % 3) - 1) * 0.7f;
}

void mostrar_estrelas() {
    for (int i = 0; i < 15; i++) {
        screenGotoxy(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
        putchar((rand() % 2) ? '.' : '*');
    }
}

void mostrar_menu(GameState *game) {
    screenClear();
    mostrar_estrelas();

    screenSetColor(YELLOW, BLACK);
    screenGotoxy(SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2 - 5);
    printf("====================");
    screenGotoxy(SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2 - 4);
    printf("    PONG DELUXE     ");
    screenGotoxy(SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2 - 3);
    printf("====================");
    screenSetColor(WHITE, BLACK);

    screenGotoxy(SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2 - 1);
    printf("> ESPAÇO - Iniciar Jogo");
    screenGotoxy(SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2);
    printf("> Q - Sair");
    screenGotoxy(SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2 + 1);
    printf("> R - Resetar Placar");

    if ((time(NULL) % 2) == 0) {
        screenSetColor(GREEN, BLACK);
        screenGotoxy(SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2 + 3);
        printf(">>> PRESSIONE ESPAÇO <<<");
        screenSetColor(WHITE, BLACK);
    }
    screenGotoxy(SCREEN_WIDTH/2 - 8, SCREEN_HEIGHT/2 + 6);
    printf("MELHORES PARTIDAS:");
    ScoreNode *atual = game->historico_placar;
    int count = 0;
    while (atual != NULL && count < 3) {
        screenGotoxy(SCREEN_WIDTH/2 - 6, SCREEN_HEIGHT/2 + 8 + count);
        printf("%d. %2d - %-2d", count + 1, atual->placar_esquerda, atual->placar_direita);
        atual = atual->next;
        count++;
    }

}



void jogo_inicio(GameState *game) {
    srand(time(NULL));

    game->campo = malloc(SCREEN_HEIGHT * sizeof(char *));
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        game->campo[i] = malloc(SCREEN_WIDTH * sizeof(char));
        memset(game->campo[i], ' ', SCREEN_WIDTH);
    }

    game->raquete_esquerda = game->raquete_direita = SCREEN_HEIGHT / 2;
    resetar_bola(game);
    game->placar_esquerda = game->placar_direita = 0;
    game->quit = false;
    game->status = MENU;
    game->jogador_vencedor = 0;

        game->placar_esquerda = game->placar_direita = 0;
    game->historico_placar = NULL;   // <-- inicializa histórico
    game->quit = false;
    game->status = MENU;
    game->jogador_vencedor = 0;

}

void liberar(GameState *game) {
    if (game->campo) {
        for (int i = 0; i < SCREEN_HEIGHT; i++) {
            free(game->campo[i]);
        }
        free(game->campo);
    }
        // liberar histórico de placares
    ScoreNode *cur = game->historico_placar;
    while (cur) {
        ScoreNode *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    game->historico_placar = NULL;

}

void add_placar_historico(GameState *game) {
    ScoreNode *novo = malloc(sizeof(ScoreNode));
    if (!novo) return;
    novo->placar_esquerda = game->placar_esquerda;
    novo->placar_direita = game->placar_direita;
    novo->next = game->historico_placar;
    game->historico_placar = novo;
}


void processar_input(GameState *game) {
    if (!keyhit()) return;
    
    int ch = readch();
    
    // ===== MENU =====
    if (game->status == MENU) {
        if (ch == ' ') game->status = PLAYING;
        else if (ch == 'q' || ch == 'Q') game->quit = true;
        return;
    }

    
    if (game->status == GAME_OVER) {
        if (ch == 'q' || ch == 'Q') {
            game->quit = true;
        }
        return; 
    }


    if (game->status == PLAYING) {
        switch(ch) {
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
            
            case 'q':
            case 'Q':
                game->status = MENU;
                resetar_bola(game);
                return;
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
            add_placar_historico(game);
            game->status = GAME_OVER;
            game->jogador_vencedor = 2;
        } else {
            resetar_bola(game);
        }
        return;
    }

    // --- Ponto para a esquerda ---
    if (game->bola_x >= SCREEN_WIDTH) {
        game->placar_esquerda++;
        
        if (game->placar_esquerda >= WINNING_SCORE) {
            add_placar_historico(game);
            game->status = GAME_OVER;
            game->jogador_vencedor = 1;

        } else {
            resetar_bola(game);
        }
        return;
    }
    
    // --- Colisão com topo/fundo --- 
    if (game->bola_y <= 0 || game->bola_y >= SCREEN_HEIGHT - 1) {
        game->bola_dir_y *= -1;
        printf("\a");
        fflush(stdout);

        return;
    }

 // --- Colisão Raquete Esquerda --- 
if (game->bola_x <= 1) {
    if (game->bola_y >= game->raquete_esquerda - 1 &&
        game->bola_y <= game->raquete_esquerda + 1) {

        float hit_pos = (game->bola_y - game->raquete_esquerda) / 2.0f;
        game->bola_dir_y = hit_pos;

        game->bola_dir_x = fabs(game->bola_dir_x); // garante que vá para direita

        contador_colisoes++;

        if (contador_colisoes % 5 == 0 && fabs(game->bola_dir_x) < MAX_SPEED) {
            game->bola_dir_x *= 1.1f;
        }

        printf("\a");
        fflush(stdout);
        return;
    }
}

// --- Colisão Raquete Direita --- 
if (game->bola_x >= SCREEN_WIDTH - 2) {
    if (game->bola_y >= game->raquete_direita - 1 &&
        game->bola_y <= game->raquete_direita + 1) {

        float hit_pos = (game->bola_y - game->raquete_direita) / 2.0f;
        game->bola_dir_y = hit_pos;

        game->bola_dir_x = -fabs(game->bola_dir_x); // garante que vá para esquerda

        contador_colisoes++;

        if (contador_colisoes % 5 == 0 && fabs(game->bola_dir_x) < MAX_SPEED) {
            game->bola_dir_x *= 1.1f;
        }

        printf("\a");
        fflush(stdout);
        return;
    }
}


}

void mostrar_game_over(GameState *game) {
    screenClear();
    mostrar_estrelas();
    
    const char* vencedor_msg = game->jogador_vencedor == 1 ? 
        "JOGADOR 1 VENCEU!" : "JOGADOR 2 VENCEU!";

    screenSetColor(YELLOW, BLACK);
    screenGotoxy(SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2 - 2);
    printf("🏆  %s  🏆", vencedor_msg);
    screenSetColor(WHITE, BLACK);

    screenGotoxy(SCREEN_WIDTH/2 - 7, SCREEN_HEIGHT/2);
    printf("Placar: %d - %d", game->placar_esquerda, game->placar_direita);

    screenGotoxy(SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2 + 2);
    printf("Pressione Q para sair");
}

void renderizar(GameState *game) {
    
    if (game->status == MENU) {
        mostrar_menu(game);
    } 
    else if (game->status == GAME_OVER) {
        mostrar_game_over(game); 
    } 
    else if (game->status == PLAYING) { 
        screenClear();

        // --- desenhar placar ---
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
        
        screenSetColor(FG_WHITE, BG_BLACK);
    } 

    screenUpdate();


}
