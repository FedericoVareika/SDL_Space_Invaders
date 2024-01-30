#ifndef BULLET_H
#define BULLET_H

#include "./definitions/global.h"
#include <stdbool.h>

typedef struct {
    int x, y;
    VertDir direction;
    bool fired;
} Bullet;

void move_bullet(float distance, Bullet *bullet);

#endif
