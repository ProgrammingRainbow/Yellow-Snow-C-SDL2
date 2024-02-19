#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"

struct Player {
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_Rect rect;
    const Uint8 *keystate;
    SDL_RendererFlip flip;
    int speed;
    int top_offset;
    int left_offset;
    int right_offset;
};

bool player_new(struct Player **player, SDL_Renderer *renderer,
                SDL_Texture *image);
void player_free(struct Player **player);
void player_update(struct Player *p);
void player_draw(struct Player *p);
int player_left(struct Player *p);
int player_right(struct Player *p);
int player_top(struct Player *p);
void player_reset(struct Player *p);

#endif
