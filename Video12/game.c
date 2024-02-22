#include "game.h"
#include "initialize.h"
#include "load_media.h"
#include "main.h"
#include "title.h"

bool check_collision(struct Game *g);
bool handle_collision(struct Game *g, struct Flake *f);
bool game_reset(struct Game *g);
Uint32 game_reset_timer(Uint32 interval, void *param);
void pause_music(struct Game *g);

bool game_new(struct Game **game) {
    *game = calloc(1, sizeof(struct Game));
    if (*game == NULL) {
        fprintf(stderr, "Error in calloc of new game.\n");
        return true;
    }
    struct Game *g = *game;

    if (game_initialize(g)) {
        return true;
    }

    if (game_load_media(g)) {
        return true;
    }

    if (player_new(&g->player, g->renderer, g->player_image)) {
        return true;
    }

    for (int i = 0; i < 5; i++) {
        if (flake_new(&g->flakes, g->renderer, g->yellow_image, false)) {
            return true;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (flake_new(&g->flakes, g->renderer, g->white_image, true)) {
            return true;
        }
    }

    if (score_new(&g->score, g->renderer)) {
        return true;
    }

    if (fps_new(&g->fps)) {
        return true;
    }

    if (title_new(&g->title, g->renderer)) {
        return true;
    }

    g->show_title = true;

    return false;
}

void game_free(struct Game **game) {
    if (*game) {
        struct Game *g = *game;

        title_free(&g->title);
        fps_free(&g->fps);
        score_free(&g->score);
        flakes_free(&g->flakes);
        player_free(&g->player);

        Mix_HaltMusic();
        Mix_FreeMusic(g->music);
        g->music = NULL;
        Mix_HaltChannel(-1);
        Mix_FreeChunk(g->collect_sound);
        g->collect_sound = NULL;
        Mix_FreeChunk(g->hit_sound);
        g->hit_sound = NULL;
        SDL_DestroyTexture(g->white_image);
        g->white_image = NULL;
        SDL_DestroyTexture(g->yellow_image);
        g->yellow_image = NULL;
        SDL_DestroyTexture(g->player_image);
        g->player_image = NULL;
        SDL_DestroyTexture(g->background_image);
        g->background_image = NULL;

        SDL_DestroyRenderer(g->renderer);
        g->renderer = NULL;
        SDL_DestroyWindow(g->window);
        g->window = NULL;

        Mix_CloseAudio();

        TTF_Quit();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();

        free(g);
        g = NULL;
        *game = NULL;
    }
}

bool game_reset(struct Game *g) {
    flakes_reset(g->flakes);
    // player_reset(g->player);

    if (score_reset(g->score)) {
        return true;
    }

    if (!g->pause_music) {
        Mix_ResumeMusic();
    }

    if (g->reset_timer_id) {
        SDL_RemoveTimer(g->reset_timer_id);
        g->reset_timer_id = 0;
    }

    title_disable(g->title);

    g->playing = true;

    return false;
}

Uint32 game_reset_timer(Uint32 interval, void *param) {
    (void)interval;
    (void)param;
    SDL_Event event;
    event.type = GAME_RESET_EVENT;
    event.user.code = 0;
    event.user.data1 = NULL;
    event.user.data2 = NULL;
    SDL_PushEvent(&event);
    return 0;
}

bool handle_collision(struct Game *g, struct Flake *f) {
    (void)g;
    if (f->is_white) {
        Mix_PlayChannel(-1, g->collect_sound, 0);
        flake_reset(f, false);
        if (score_increment(g->score)) {
            return true;
        }

    } else {
        Mix_PauseMusic();
        Mix_PlayChannel(-1, g->hit_sound, 0);
        g->playing = false;
        g->reset_timer_id = SDL_AddTimer(5000, game_reset_timer, NULL);
    }

    return false;
}

bool check_collision(struct Game *g) {
    struct Flake *f = g->flakes;
    int p_top = player_top(g->player);
    int p_left = player_left(g->player);
    int p_right = player_right(g->player);

    while (f) {
        if (flake_bottom(f) > p_top) {
            if (flake_right(f) > p_left) {
                if (flake_left(f) < p_right) {
                    if (handle_collision(g, f)) {
                        return true;
                    }
                }
            }
        }
        f = f->next;
    }

    return false;
}

void pause_music(struct Game *g) {
    if (g->pause_music) {
        g->pause_music = false;
        if (g->playing || g->title->show_title) {
            Mix_ResumeMusic();
        }
    } else {
        g->pause_music = true;
        Mix_PauseMusic();
    }
}

bool game_run(struct Game *g) {

    if (Mix_PlayMusic(g->music, -1)) {
        fprintf(stderr, "Error playing Music: %s\n", Mix_GetError());
        return true;
    }

    while (true) {
        while (SDL_PollEvent(&g->event)) {
            switch (g->event.type) {
            case SDL_QUIT:
                return false;
                break;
            case GAME_RESET_EVENT:
                title_reset(g->title);
                if (!g->pause_music) {
                    Mix_ResumeMusic();
                }
                break;
            case SDL_KEYDOWN:
                switch (g->event.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    return false;
                    break;
                case SDL_SCANCODE_SPACE:
                    if (!g->playing) {
                        if (game_reset(g)) {
                            return true;
                        }
                    }
                    break;
                case SDL_SCANCODE_F:
                    fps_toggle_display(g->fps);
                    break;
                case SDL_SCANCODE_P:
                    pause_music(g);
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }

        if (g->playing || g->title->show_title) {
            flakes_update(g->flakes, g->delta_time);
        }

        if (g->playing) {
            player_update(g->player, g->delta_time);
            if (check_collision(g)) {
                return true;
            }
        } else {
            title_update(g->title, g->delta_time);
        }

        SDL_RenderClear(g->renderer);

        SDL_RenderCopy(g->renderer, g->background_image, NULL,
                       &g->background_rect);

        player_draw(g->player);

        flakes_draw(g->flakes);

        score_draw(g->score);

        title_draw(g->title);

        SDL_RenderPresent(g->renderer);

        g->delta_time = fps_update(g->fps);
    }

    return false;
}