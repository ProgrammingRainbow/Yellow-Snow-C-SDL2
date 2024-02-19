#include "initialize.h"

bool game_initialize(struct Game *g) {
    if (SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return true;
    }

    g->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                 WINDOW_HEIGHT, 0);
    if (!g->window) {
        fprintf(stderr, "Error creating Window: %s\n", SDL_GetError());
        return true;
    }

    g->renderer = SDL_CreateRenderer(g->window, -1, SDL_RENDERER_ACCELERATED);
    if (!g->renderer) {
        fprintf(stderr, "Error creating Renderer: %s\n", SDL_GetError());
        return true;
    }

    SDL_Surface *icon_surf = IMG_Load(WINDOW_ICON);
    if (icon_surf) {
        SDL_SetWindowIcon(g->window, icon_surf);
        SDL_FreeSurface(icon_surf);
        icon_surf = NULL;
    } else {
        fprintf(stderr, "Error loading icon Surface: %s\n", IMG_GetError());
        return true;
    }

    srand((unsigned)time(NULL));

    return false;
}
