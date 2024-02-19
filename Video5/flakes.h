#ifndef FLAKES_H
#define FLAKES_H

#include "main.h"

struct Flake {
    struct Flake *next;
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_Rect rect;
};

bool flake_new(struct Flake **flake, SDL_Renderer *renderer,
               SDL_Texture *image);
void flakes_free(struct Flake **flakes);
void flakes_update(struct Flake *f);
void flakes_draw(struct Flake *f);

#endif
