#ifndef ALIEN_H
#define ALIEN_H

#include "./bullet.h"
#include "./definitions/settings.h"
#include "definitions/global.h"
#include <SDL2/SDL.h>

typedef struct {
    int x, y;
    int w, h;
    int cols;
    int aliens[AMOUNT_ALIEN_COLS][AMOUNT_ALIEN_ROWS];
    Bullet bullets[MAX_BULLETS_ALIENS];
} Aliens;

int get_aliens_width();
int get_aliens_height();
void setup_aliens(Aliens *aliens);
void move_aliens(float distance, HorDir *dir, Aliens *aliens);
void render_aliens(Aliens aliens, SDL_Renderer *renderer);
void kill_alien(float x, float y, Aliens *aliens, Bullet *bullet);
void update_aliens_width(Aliens *aliens);

#endif
