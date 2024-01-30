#include "./bullet.h"

void move_bullet(float distance, Bullet *bullet) {
    if (!bullet->fired) {
        return;
    }

    switch (bullet->direction) {
    case UP:
        bullet->y -= distance;
        break;
    case DOWN:
        bullet->y += distance;
        break;
    }
}
