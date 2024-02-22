#include "flakes.h"

bool flake_new(struct Flake **flake, SDL_Renderer *renderer, SDL_Texture *image,
               bool is_white) {
    struct Flake *new_flake = calloc(1, sizeof(struct Flake));
    if (!new_flake) {
        fprintf(stderr, "Error in calloc of new flake.\n");
        return true;
    }

    new_flake->renderer = renderer;
    new_flake->image = image;
    new_flake->is_white = is_white;
    new_flake->speed = 300;
    new_flake->ground = 514;

    if (SDL_QueryTexture(new_flake->image, NULL, NULL, &new_flake->rect.w,
                         &new_flake->rect.h)) {
        fprintf(stderr, "Error querying Texture: %s\n", SDL_GetError());
        return true;
    }

    flake_reset(new_flake, true);

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

void flake_reset(struct Flake *f, bool full) {
    int height = full ? WINDOW_HEIGHT * 2 : WINDOW_HEIGHT;
    f->rect.y = -((rand() % height) + f->rect.h);
    f->pos_y = (double)f->rect.y;
    f->rect.x = (rand() % (WINDOW_WIDTH - f->rect.w));
}

void flakes_reset(struct Flake *f) {
    while (f) {
        flake_reset(f, true);
        f = f->next;
    }
}

int flake_left(struct Flake *f) { return f->rect.x; }

int flake_right(struct Flake *f) { return f->rect.x + f->rect.w; }

int flake_bottom(struct Flake *f) { return f->rect.y + f->rect.h; }

void flakes_update(struct Flake *f, double dt) {
    while (f) {
        f->pos_y += f->speed * dt;
        if (f->pos_y > f->ground) {
            flake_reset(f, false);
        } else {
            f->rect.y = (int)f->pos_y;
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
