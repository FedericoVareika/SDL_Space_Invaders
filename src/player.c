#include "./player.h"
#include "definitions/constants.h"
#include "definitions/global.h"
#include <stdbool.h>

void move_player(float distance, HorDir dir, Player *player) {
    switch (dir) {
    case LEFT:
        player->x -= distance;
        break;
    case RIGHT:
        player->x += distance;
        break;
    }
}

void render_player(Player player, SDL_Renderer *renderer) {
    float paddedX = player.x + LEFT_PADDING;
    float paddedY = player.y + NORTH_PADDING;
    SDL_Rect player_rect = {paddedX, paddedY, PLAYER_WIDTH, PLAYER_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &player_rect);
}

void shoot_bullet(Player *player) {
    for (int i = 0; i < MAX_BULLETS_PLAYER; i++) {
        if (!player->bullets[i].fired) {
            Bullet *bullet = &player->bullets[i];
            bullet->fired = true;
            bullet->x = player->x + (PLAYER_WIDTH - BULLET_WIDTH) / 2.0f;
            bullet->y = player->y - (BULLET_HEIGHT / 2.0f);
            i = MAX_BULLETS_PLAYER;
        }
    }
}
