#ifndef FLAKES_H
#define FLAKES_H

#include "main.h"

struct Flake {
    struct Flake *next;
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_Rect rect;
    int speed;
    bool is_white;
    int ground;
};

bool flake_new(struct Flake **flake, SDL_Renderer *renderer,
               SDL_Texture *image);
void flakes_free(struct Flake **flakes);
void flakes_update(struct Flake *f);
void flakes_draw(struct Flake *f);
void flake_reset(struct Flake *f, bool full);
void flakes_reset(struct Flake *f);
int flake_left(struct Flake *f);
int flake_right(struct Flake *f);
int flake_bottom(struct Flake *f);

#endif
