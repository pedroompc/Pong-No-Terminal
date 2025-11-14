#include "../cli-lib/include/screen.h"
#include "../cli-lib/include/keyboard.h"
#include "../cli-lib/include/timer.h"

int main() {
    screenInit(1);
    screenHideCursor();
    keyboardInit();
    timerInit(20);

    while (1) {
        if (keyhit()) {
            int ch = readch();
            if (ch == 'q' || ch == 'Q') break;
        }

        if (timerTimeOver()) {
            screenClear();
            screenGotoxy(10, 10);
            printf("Pong base - pressione Q para sair");
            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    return 0;
}
