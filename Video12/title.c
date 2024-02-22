#include "title.h"
#include <SDL2/SDL_surface.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

bool title_new(struct Title **title, SDL_Renderer *renderer) {
    *title = calloc(1, sizeof(struct Title));
    if (*title == NULL) {
        fprintf(stderr, "Error in calloc of new title.\n");
        return true;
    }
    struct Title *t = *title;

    t->renderer = renderer;
    t->speed = 60;
    SDL_Color outer_color = {255, 255, 0, 255};
    SDL_Color center_color = {255, 255, 255, 255};
    int text_size = 100;
    int radius = 15;
    const char *text = "Yellow Snow";

    t->font = TTF_OpenFont("fonts/freesansbold.ttf", text_size);
    if (!t->font) {
        fprintf(stderr, "Error opening Font: %s\n", TTF_GetError());
        return true;
    }

    t->text_surf = TTF_RenderText_Blended(t->font, text, outer_color);
    if (!t->text_surf) {
        fprintf(stderr, "Error creating text Surface: %s\n", TTF_GetError());
        return true;
    }

    int padding = radius * 2;
    t->target_surf = SDL_CreateRGBSurface(
        0, t->text_surf->w + padding, t->text_surf->h + padding, 32, 0x00FF0000,
        0x0000FF00, 0x000000FF, 0xFF000000);
    if (!t->target_surf) {
        fprintf(stderr, "Error creating text Surface: %s\n", TTF_GetError());
        return true;
    }

    t->rect.w = t->target_surf->w;
    t->rect.h = t->target_surf->h;

    // Polar Coordinates Trigonometry Algorithm
    for (int index = 0; index < (int)(2 * M_PI * radius); index++) {
        double rad = (double)index / radius;
        int x = (int)(cos(rad) * radius) + radius;
        int y = (int)(sin(rad) * radius) + radius;
        SDL_BlitSurface(t->text_surf, NULL, t->target_surf,
                        &(SDL_Rect){x, y, t->text_surf->w, t->text_surf->h});
    }

    SDL_FreeSurface(t->text_surf);
    t->text_surf = NULL;
    t->text_surf = TTF_RenderText_Blended(t->font, text, center_color);
    if (!t->text_surf) {
        fprintf(stderr, "Error creating text Surface: %s\n", TTF_GetError());
        return true;
    }

    SDL_BlitSurface(
        t->text_surf, NULL, t->target_surf,
        &(SDL_Rect){radius, radius, t->text_surf->w, t->text_surf->h});

    t->image = SDL_CreateTextureFromSurface(t->renderer, t->target_surf);
    if (!t->image) {
        fprintf(stderr, "Error creating Texture from Surface: %s\n",
                SDL_GetError());
        return true;
    }

    SDL_FreeSurface(t->text_surf);
    t->text_surf = NULL;
    SDL_FreeSurface(t->target_surf);
    t->target_surf = NULL;

    t->rect.x = (WINDOW_WIDTH - t->rect.w) / 2;
    t->target_y = (double)(WINDOW_HEIGHT - t->rect.h) / 2;
    title_reset(t);

    return false;
}

void title_free(struct Title **title) {
    if (*title) {
        struct Title *t = *title;
        TTF_CloseFont(t->font);
        t->font = NULL;
        SDL_DestroyTexture(t->image);
        t->image = NULL;
        SDL_FreeSurface(t->text_surf);
        t->text_surf = NULL;
        SDL_FreeSurface(t->target_surf);
        t->target_surf = NULL;
        t->renderer = NULL;
        free(t);
        t = NULL;
        *title = NULL;
    }
}

void title_reset(struct Title *t) {
    t->rect.y = -t->rect.h;
    t->pos_y = t->rect.y;

    t->show_title = true;
    t->show_intro = true;
}

void title_update(struct Title *t, double dt) {
    if (t->show_intro) {
        t->pos_y += t->speed * dt;
        if (t->pos_y < t->target_y) {
            t->rect.y = (int)t->pos_y;
        } else {
            t->show_intro = false;
        }
    }
}

void title_disable(struct Title *t) {
    t->show_intro = false;
    t->show_title = false;
}

void title_draw(struct Title *t) {
    if (t->show_title) {
        SDL_RenderCopy(t->renderer, t->image, NULL, &t->rect);
    }
}
