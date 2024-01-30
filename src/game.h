#ifndef GAME_H
#define GAME_H

#include "alien.h"
#include "definitions/settings.h"
#include "player.h"
#include <SDL2/SDL.h>

typedef struct {
    Player player;
    Aliens aliens;
} Game;

void init_game(Game *game);
void move_bullets(float distance, Game *game);
bool detect_collision(SDL_Rect r1, SDL_Rect r2);
void kill_hit_aliens(Game *game);
void check_collisions(Game *game);
void render_bullets(Game game, SDL_Renderer *renderer);
void quit_game(Game *game);

#endif
