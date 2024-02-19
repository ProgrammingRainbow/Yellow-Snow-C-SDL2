#include "flakes.h"

bool flake_new(struct Flake **flake, SDL_Renderer *renderer,
               SDL_Texture *image) {
    struct Flake *new_flake = calloc(1, sizeof(struct Flake));
    if (!new_flake) {
        fprintf(stderr, "Error in calloc of new flake.\n");
        return true;
    }

    new_flake->renderer = renderer;
    new_flake->image = image;

    if (SDL_QueryTexture(new_flake->image, NULL, NULL, &new_flake->rect.w,
                         &new_flake->rect.h)) {
        fprintf(stderr, "Error querying Texture: %s\n", SDL_GetError());
        return true;
    }

    new_flake->rect.x = (rand() % (WINDOW_WIDTH - new_flake->rect.w));

    new_flake->next = *flake;
    *flake = new_flake;

    return false;
}

void flakes_free(struct Flake **flakes) {
    struct Flake *f = *flakes;
    while (f) {
        struct Flake *next = f->next;
        f->image = NULL;
        f->renderer = NULL;
        f->next = NULL;
        free(f);
        f = next;
    }
    *flakes = NULL;
}

void flakes_update(struct Flake *f) {
    while (f) {
        f->rect.y += 5;
        if (f->rect.y > 514) {
            f->rect.y = 0;
        }
        f = f->next;
    }
}

void flakes_draw(struct Flake *f) {
    while (f) {
        SDL_RenderCopy(f->renderer, f->image, NULL, &f->rect);
        f = f->next;
    }
}
