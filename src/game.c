#include "./game.h"
#include "./definitions/constants.h"
#include "./definitions/settings.h"
#include "alien.h"
#include "bullet.h"
#include "definitions/global.h"
#include "player.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

void init_game(Game *game) {
    // Game *game = malloc(sizeof(Game));

    // init player
    game->player.x = (ARENA_WIDTH - PLAYER_WIDTH) / 2.0f;
    game->player.y = ARENA_HEIGHT - PLAYER_HEIGHT / 2.0f;
    game->player.lives = PLAYER_LIVES;

    // init player bullets
    for (int i = 0; i < MAX_BULLETS_PLAYER; i++) {
        game->player.bullets[i].direction = UP;
        game->player.bullets[i].fired = false;
    }

    // init aliens
    game->aliens.x = (ARENA_WIDTH - get_aliens_width()) / 2.0f;
    game->aliens.y = ALIENS_STARTING_Y;
    game->aliens.w =
        (ALIEN_WIDTH + ALIEN_SPACING) * AMOUNT_ALIEN_COLS - ALIEN_SPACING;
    game->aliens.h =
        (ALIEN_HEIGHT + ALIEN_SPACING) * AMOUNT_ALIEN_ROWS - ALIEN_SPACING;
    game->aliens.cols = AMOUNT_ALIEN_COLS;
}

void quit_game(Game *game) {
    free(game);
}

bool detect_collision(SDL_Rect r1, SDL_Rect r2) {
    bool x_intersect =
        max(r1.x + r1.w, r2.x + r2.w) - min(r1.x, r2.x) < r1.w + r2.w;
    bool y_intersect =
        max(r1.y + r1.h, r2.y + r2.h) - min(r1.y, r2.y) < r1.h + r2.h;

    return x_intersect && y_intersect;
}

void kill_hit_aliens(Game *game) {
    Aliens aliens = game->aliens;
    SDL_Rect aliens_rect = {aliens.x, aliens.y, aliens.w, aliens.h};
    for (int i = 0; i < MAX_BULLETS_PLAYER; i++) {
        Bullet bullet = game->player.bullets[i];
        if (bullet.fired) {
            SDL_Rect bullet_rect = {
                bullet.x, bullet.y, BULLET_WIDTH, BULLET_HEIGHT};
            // if the bullet is inside the area where the aliens are
            if (detect_collision(aliens_rect, bullet_rect)) {
                // get the relative position of the bullet to the alien
                // rectangle
                float relative_bullet_x = bullet.x - aliens.x;
                float relative_bullet_y = bullet.y - aliens.y;

                // kill the alien it is over (if it is over any alive)
                kill_alien(relative_bullet_x,
                           relative_bullet_y,
                           &game->aliens,
                           &game->player.bullets[i]);
            }
        }
    }
}

void check_collisions(Game *game) {
    // Player against left and right walls
    if (game->player.x < LEFT_WALL) {
        game->player.x = LEFT_WALL;
    } else if (game->player.x + PLAYER_WIDTH > RIGHT_WALL) {
        game->player.x = RIGHT_WALL - PLAYER_WIDTH;
    }

    // Player bullets against north wall
    for (int i = 0; i < MAX_BULLETS_PLAYER; i++) {
        if (game->player.bullets[i].fired &&
            game->player.bullets[i].y <= NORTH_WALL) {
            game->player.bullets[i].fired = false;
        }
    }

    // Alien bullets against south wall
    for (int i = 0; i < MAX_BULLETS_ALIENS; i++) {
        if (game->aliens.bullets[i].fired &&
            game->aliens.bullets[i].y >= SOUTH_WALL) {
            game->aliens.bullets[i].fired = false;
        }
    }

    kill_hit_aliens(game);
    update_aliens_width(&game->aliens);
}

void render_bullets(Game game, SDL_Renderer *renderer) {
    for (int i = 0; i < MAX_BULLETS_PLAYER; i++) {
        Bullet bullet = game.player.bullets[i];
        if (bullet.fired) {
            float paddedX = bullet.x + LEFT_PADDING;
            float paddedY = bullet.y + NORTH_PADDING;
            SDL_Rect bullet_rect = {
                paddedX, paddedY, BULLET_WIDTH, BULLET_HEIGHT};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &bullet_rect);
        }
    }

    // TODO: Render alien bullets
}

void move_bullets(float distance, Game *game) {
    for (int i = 0; i < MAX_BULLETS_PLAYER; i++) {
        move_bullet(distance, &game->player.bullets[i]);
    }

    for (int i = 0; i < MAX_BULLETS_ALIENS; i++) {
        move_bullet(distance, &game->aliens.bullets[i]);
    }
}
