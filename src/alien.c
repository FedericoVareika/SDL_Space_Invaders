#include "./alien.h"
#include "./definitions/constants.h"

int get_aliens_width() {
    return (ALIEN_WIDTH + ALIEN_SPACING) * AMOUNT_ALIEN_COLS - ALIEN_SPACING;
}

int get_aliens_height() {
    return (ALIEN_HEIGHT + ALIEN_SPACING) * AMOUNT_ALIEN_ROWS - ALIEN_SPACING;
}

void setup_aliens(Aliens *aliens) {
    for (int i = 0; i < aliens->cols; i++) {
        for (int j = 0; j < AMOUNT_ALIEN_ROWS; j++) {
            aliens->aliens[i][j] = true;
        }
    }

    for (int i = 0; i < MAX_BULLETS_ALIENS; i++) {
        aliens->bullets[i].direction = DOWN;
        aliens->bullets[i].fired = false;
    }
}

void move_aliens(float distance, HorDir *dir, Aliens *aliens) {
    switch (*dir) {
    case RIGHT:
        aliens->x += distance;
        if (aliens->x + aliens->w > RIGHT_WALL) {
            aliens->x = RIGHT_WALL - aliens->w;
            aliens->y += PIXEL_LENGTH;
            *dir = LEFT;
        }
        break;
    case LEFT:
        aliens->x -= distance;
        if (aliens->x < LEFT_WALL) {
            aliens->x = LEFT_WALL;
            aliens->y += PIXEL_LENGTH;
            *dir = RIGHT;
        }
        break;
    }
}

void render_aliens(Aliens aliens, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    float paddedX = aliens.x + LEFT_PADDING;
    float paddedY = aliens.y + NORTH_PADDING;
    for (int i = 0; i < aliens.cols; i++) {
        for (int j = 0; j < AMOUNT_ALIEN_ROWS; j++) {
            if (aliens.aliens[i][j]) {
                int x = paddedX + ((ALIEN_WIDTH + ALIEN_SPACING) * i);
                int y = paddedY + ((ALIEN_HEIGHT + ALIEN_SPACING) * j);
                SDL_Rect alien_rect = {x, y, ALIEN_WIDTH, ALIEN_HEIGHT};
                SDL_RenderFillRect(renderer, &alien_rect);
            }
        }
    }
}

void kill_alien(float x, float y, Aliens *aliens, Bullet *bullet) {
    // Go through each alien column
    for (int i = 0; i < aliens->cols; i++) {
        // if the alien.x is at pos x (accounting for the spacing)
        if (x <= (ALIEN_WIDTH + ALIEN_SPACING) * (i + 1) - ALIEN_SPACING &&
            x + BULLET_WIDTH > (ALIEN_WIDTH + ALIEN_SPACING) * i) {
            for (int j = 0; j < AMOUNT_ALIEN_ROWS; j++) {
                // if the alien.y is at pos y (accounting for the spacing)
                if (y <= (ALIEN_HEIGHT + ALIEN_SPACING) * (j + 1) -
                             ALIEN_SPACING &&
                    y > (ALIEN_HEIGHT + ALIEN_SPACING) * j &&
                    aliens->aliens[i][j]) {
                    aliens->aliens[i][j] = false;
                    bullet->fired = false;
                    return;
                }
            }
            return;
        }
    }
}

void shift_left(Aliens *aliens) {
    for (int i = 1; i < aliens->cols + 1; i++) {
        for (int j = 0; j < AMOUNT_ALIEN_ROWS; j++) {
            aliens->aliens[i - 1][j] = aliens->aliens[i][j];
        }
    }
}

void update_aliens_width(Aliens *aliens) {
    bool empty_col = true;
    for (int i = 0; i < AMOUNT_ALIEN_ROWS; i++) {
        if (aliens->aliens[0][i]) {
            empty_col = false;
            break;
        }
    }
    if (empty_col) {
        aliens->x += ALIEN_WIDTH + ALIEN_SPACING;
        aliens->w -= ALIEN_WIDTH + ALIEN_SPACING;
        aliens->cols -= 1;
        shift_left(aliens);
    }

    empty_col = true;
    for (int i = 0; i < AMOUNT_ALIEN_ROWS; i++) {
        if (aliens->aliens[aliens->cols - 1][i]) {
            empty_col = false;
            break;
        }
    }
    if (empty_col) {
        aliens->w -= ALIEN_WIDTH + ALIEN_SPACING;
        aliens->cols--;
    }
}
