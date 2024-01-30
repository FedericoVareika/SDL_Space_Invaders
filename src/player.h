#ifndef PLAYER_H
#define PLAYER_H

#include "./bullet.h"
#include "./definitions/global.h"
#include "./definitions/settings.h"
#include <SDL2/SDL.h>

typedef struct {
    float x, y;
    Bullet bullets[MAX_BULLETS_PLAYER];
    int lives;
} Player;

void move_player(float distance, HorDir dir, Player *player);
void render_player(Player player, SDL_Renderer *renderer);
void shoot_bullet(Player *player);

#endif
