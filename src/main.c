#include "./definitions/constants.h"
#include "./game.h"
#include "alien.h"
#include "definitions/global.h"
#include "player.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

bool game_is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int last_frame_time = 0;
int last_game_update_time = 0;
float delta_time = 0.0f;

Game *game;
bool player_moving = false;
HorDir player_moving_direction = LEFT;
bool player_shooting = false;

HorDir alien_moving_direction = LEFT;
int last_alien_move_time = 0;
int big_pixels_per_alien_move = 1;

bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    window = SDL_CreateWindow(NULL,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              ARENA_WIDTH + LEFT_PADDING + RIGHT_PADDING,
                              ARENA_HEIGHT + NORTH_PADDING + SOUTH_PADDING,
                              SDL_WINDOW_BORDERLESS);

    if (!window) {
        fprintf(stderr, "Error creating SDL window. \n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer. \n");
        return false;
    }

    return true;
}

void destroy_window() {
    quit_game(game);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void process_input() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        game_is_running = false;
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            game_is_running = false;
            break;
        case SDLK_LEFT:
            player_moving = true;
            player_moving_direction = LEFT;
            break;
        case SDLK_RIGHT:
            player_moving = true;
            player_moving_direction = RIGHT;
            break;
        case SDLK_SPACE:
            player_shooting = true;
            break;
        }
        break;
    case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
            if (player_moving_direction == LEFT) {
                player_moving = false;
            }
            break;
        case SDLK_RIGHT:
            if (player_moving_direction == RIGHT) {
                player_moving = false;
            }
            break;
        }
    }
}

void setup() {
    game = malloc(sizeof(Game));
    if (!game) {
        fprintf(stderr, "Failed to allocate memory for game.\n");
        game_is_running = false;
        return;
    }

    init_game(game);
    setup_aliens(&game->aliens);
    printf("Game initialized!");
}

void do_actions() {
    if (player_moving) {
        move_player(
            PLAYER_SPEED * delta_time, player_moving_direction, &game->player);
    }
    if (player_shooting) {
        shoot_bullet(&game->player);
        player_shooting = false;
    }
}

void standard_movement() {
    if (ALIEN_SPEED * (SDL_GetTicks() - last_alien_move_time) >=
        big_pixels_per_alien_move * PIXEL_LENGTH * 1000.0f) {
        move_aliens(PIXEL_LENGTH, &alien_moving_direction, &game->aliens);
        last_alien_move_time = SDL_GetTicks();
    }
    move_bullets(BULLET_SPEED * delta_time, game);
}

void update() {
    // int delay = last_frame_time + FRAME_TARGET_TIME - SDL_GetTicks();
    // if (delay >= 0 && delay < FRAME_TARGET_TIME)
    //     SDL_Delay(delay);
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    do_actions();

    standard_movement();

    check_collisions(game);

    last_frame_time = SDL_GetTicks();
}

void render() {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    render_player(game->player, renderer);
    render_aliens(game->aliens, renderer);
    render_bullets(*game, renderer);

    // Buffers swap (back buffer and front buffer)
    SDL_RenderPresent(renderer);
}

void run_game() {
    game_is_running = initialize_window();
    setup();

    while (game_is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();
}

int main() {
    run_game();
    return 0;
}
