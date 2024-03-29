#include "load_media.h"

bool game_load_media(struct Game *g) {
    g->background_image = IMG_LoadTexture(g->renderer, "images/background.png");
    if (!g->background_image) {
        fprintf(stderr, "Error loading Texture: %s\n", IMG_GetError());
        return true;
    }

    if (SDL_QueryTexture(g->background_image, NULL, NULL, &g->background_rect.w,
                         &g->background_rect.h)) {
        fprintf(stderr, "Error querying Texture: %s\n", SDL_GetError());
        return true;
    }

    g->player_image = IMG_LoadTexture(g->renderer, "images/player.png");
    if (!g->player_image) {
        fprintf(stderr, "Error loading Texture: %s\n", IMG_GetError());
        return true;
    }

    return false;
}
