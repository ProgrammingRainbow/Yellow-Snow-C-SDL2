#ifndef GAME_H
#define GAME_H

#include "main.h"

struct Game {
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background_image;
    SDL_Rect background_rect;
};

bool game_new(struct Game **game);
void game_free(struct Game **game);
bool game_run(struct Game *g);

#endif
