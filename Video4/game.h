#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "player.h"

struct Game {
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background_image;
    SDL_Rect background_rect;
    SDL_Texture *player_image;
    struct Player *player;
};

bool game_new(struct Game **game);
void game_free(struct Game **game);
bool game_run(struct Game *g);

#endif
