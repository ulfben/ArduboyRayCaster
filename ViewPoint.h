#pragma once
#include "Config.h"
#include "LevelData.h"
#include <Arduboy2.h>
struct ViewPoint {
    int x = 0, y = 0, angle = 0;
    float dx = 0.0f, dy = 0.0f;
    ViewPoint(int cellx, int celly, int startAngle): angle { startAngle }{
        centerInCell(cellx, celly);
    }
    void update(Arduboy2& a){
        dx = 0.0f;
        dy = 0.0f;
        if (a.pressed(RIGHT_BUTTON)) {
            if ((angle += Cfg::ROTATION_SPEED) >= ANGLE_360) {
                angle = ANGLE_0;
            }
        } else if (a.pressed(LEFT_BUTTON)) {
            if ((angle -= Cfg::ROTATION_SPEED) < ANGLE_0) {
                angle = ANGLE_360;
            }
        }
        if (a.pressed(UP_BUTTON)) {
            dx = cos(angle * ANGLE_TO_RADIANS) * Cfg::WALK_SPEED;
            dy = sin(angle * ANGLE_TO_RADIANS) * Cfg::WALK_SPEED;
        } else if (a.pressed(DOWN_BUTTON)) {
            dx = -cos(angle * ANGLE_TO_RADIANS) * Cfg::WALK_SPEED;
            dy = -sin(angle * ANGLE_TO_RADIANS) * Cfg::WALK_SPEED;
        }
        x += static_cast<int>(dx);
        y += static_cast<int>(dy);
    }

    inline constexpr void centerInCell(int cellx, int celly) noexcept  {
        x = cellx * CELL_SIZE + (CELL_SIZE >> 1);
        y = celly * CELL_SIZE + (CELL_SIZE >> 1);
    }
    
    void checkCollisions() noexcept {// test if user has bumped into a wall, and if so, back up!    
        const int MAX = CELL_SIZE - OVERBOARD;
        const int MIN = OVERBOARD;
        const int x_cell = x / CELL_SIZE;
        const int y_cell = y / CELL_SIZE;
        const int x_sub_cell = x % CELL_SIZE; // compute position within the cell
        const int y_sub_cell = y % CELL_SIZE;
        if (dx > 0 && isWall(x_cell + 1, y_cell)) { // moving right, towards a wall
            if (x_sub_cell > MAX) {
                x -= (x_sub_cell - MAX); // back player up amount they stepped over the line
            }
        } else if (dx < 0 && isWall(x_cell - 1, y_cell)) { // moving left, towards a wall
            if (x_sub_cell < MIN) {
                x += (MIN - x_sub_cell);
            }
        }
        if (dy > 0 && isWall(x_cell, y_cell + 1)) { // moving up
            if (y_sub_cell > MAX) {
                y -= (y_sub_cell - MAX);
            }
        } else if (dy < 0 && isWall(x_cell, y_cell - 1)) { // moving down
            if (y_sub_cell < MIN) {
                y += (MIN - y_sub_cell);
            }
        }
    }
};
