#include "game.h"
#include "initialize.h"
#include "load_media.h"

bool game_new(struct Game **game) {
    *game = calloc(1, sizeof(struct Game));
    if (*game == NULL) {
        fprintf(stderr, "Error in calloc of new game.\n");
        return true;
    }
    struct Game *g = *game;

    if (game_initialize(g)) {
        return true;
    }

    if (game_load_media(g)) {
        return true;
    }

    return false;
}

void game_free(struct Game **game) {
    if (*game) {
        struct Game *g = *game;

        SDL_DestroyTexture(g->background_image);
        g->background_image = NULL;

        SDL_DestroyRenderer(g->renderer);
        g->renderer = NULL;
        SDL_DestroyWindow(g->window);
        g->window = NULL;

        SDL_Quit();

        free(g);
        g = NULL;
        *game = NULL;
    }
}

bool game_run(struct Game *g) {
    while (true) {
        while (SDL_PollEvent(&g->event)) {
            switch (g->event.type) {
            case SDL_QUIT:
                return false;
                break;
            case SDL_KEYDOWN:
                switch (g->event.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    return false;
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
        SDL_RenderClear(g->renderer);

        SDL_RenderCopy(g->renderer, g->background_image, NULL,
                       &g->background_rect);

        SDL_RenderPresent(g->renderer);

        SDL_Delay(16);
    }

    return false;
}
