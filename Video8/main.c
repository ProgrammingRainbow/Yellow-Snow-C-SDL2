#include "game.h"

int main(void) {
    struct Game *game = NULL;

    if (game_new(&game) || game_run(game)) {
        game_free(&game);
        return EXIT_FAILURE;
    }

    game_free(&game);
    return EXIT_SUCCESS;
}
