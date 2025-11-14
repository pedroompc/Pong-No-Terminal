#include "game.h"
#include "../cli-lib/include/screen.h"
#include "../cli-lib/include/keyboard.h"
#include "../cli-lib/include/timer.h"

int main() {
    screenInit(1);
    screenHideCursor();
    keyboardInit();
    timerInit(20);

    GameState game;
    jogo_inicio(&game);

    while (!game.quit) {
        processar_input(&game);

        if (timerTimeOver()) {
            atualizar_jogo(&game);
            renderizar(&game);
        }
    }

    keyboardDestroy();
    screenDestroy();
    liberar(&game);
    return 0;
}
