#include "Player.h"

bool Player::Moved() const {
    if (coords.x == old_coords.x && coords.y == old_coords.y) {
        return false;
    } else {
        return true;
    }
}


void Player::ProcessInput(MovementDir dir, char room_buff[25][25], char *curr_room_ch, bool *trap_activated,
                                                                    bool *win, bool *lose) {
    int move_dist = move_speed * 1;
    int xdiff = 0, ydiff = 0;
    if (dir == MovementDir::UP) {
        ydiff += move_dist;
    } else if (dir == MovementDir::DOWN) {
        ydiff -= move_dist;
    } else if (dir == MovementDir::LEFT) {
        xdiff -= move_dist;
    } else if (dir == MovementDir::RIGHT) {
        xdiff += move_dist;
    }
    int scrlen = tileSize * tileNum;
    int scrlen2 = tileSize * (tileNum - 2);
    char left_bot = room_buff[(scrlen - (coords.y + ydiff)) / tileSize][(coords.x + xdiff) / tileSize];
    char left_top = room_buff[(scrlen2 - (coords.y + ydiff)) / tileSize + 1][(coords.x + xdiff) / tileSize];
    char right_bot = room_buff[(scrlen - (coords.y + ydiff)) / tileSize][(coords.x + xdiff) / tileSize + 1];
    char right_top = room_buff[(scrlen2 - (coords.y + ydiff)) / tileSize + 1][(coords.x + xdiff) / tileSize + 1];
    bool wall = false;
    if (dir == MovementDir::UP) {
        if (left_top == '#' || right_top == '#') {
            wall = true;
        }
        if (left_top == 'a' || right_top == 'a') {
            *trap_activated = true;
        }
        if (left_top == '!' || right_top == '!') {
            *win = true;
        }
        if (*trap_activated && (left_top == 't' || right_top == 't')) {
            *lose = true;
        }
        if (left_top == 'U' && right_top == 'U') {
            *curr_room_ch = 'U';
        }
        old_coords.y = coords.y;
    } else if (dir == MovementDir::DOWN) {
        if (left_bot == '#' || left_bot == '#') {
            wall = true;
        }
        if (left_bot == 'a' || left_bot == 'a') {
            *trap_activated = true;
        }
        if (left_bot == '!' || left_bot == '!') {
            *win = true;
        }
        if (*trap_activated && (left_bot == 't' || left_bot == 't')) {
            *lose = true;
        }
        if (left_bot == 'D' && right_bot == 'D') {
            *curr_room_ch = 'D';
        }
        old_coords.y = coords.y;
    } else if (dir == MovementDir::LEFT) {
        if (left_top == '#' || left_bot == '#') {
            wall = true;
        }
        if (left_top == 'a' || left_bot == 'a') {
            *trap_activated = true;
        }
        if (left_top == '!' || left_bot == '!') {
            *win = true;
        }
        if (*trap_activated && (left_top == 't' || left_bot == 't')) {
            *lose = true;
        }
        if (left_top == 'L' && left_bot == 'L') {
            *curr_room_ch = 'L';
        }
        old_coords.x = coords.x;
    } else if (dir == MovementDir::RIGHT) {
        if (right_bot == '#' || right_top == '#') {
            wall = true;
        }
        if (right_bot == 'a' || right_top == 'a') {
            *trap_activated = true;
        }
        if (right_bot == '!' || right_top == '!') {
            *win = true;
        }
        if (*trap_activated && (right_bot == 't' || right_top == 't')) {
            *lose = true;
        }
        if (right_bot == 'R' && right_top == 'R') {
            *curr_room_ch = 'R';
        }
        old_coords.x = coords.x;
    }
    if (!wall) {
        coords.y += ydiff;
        coords.x += xdiff;
    }
}

void Player::changePos(char *curr_room_ch) {
    if (*curr_room_ch == 'U') {
        coords.x = tileSize * (tileNum / 2);
        coords.y = tileSize * 2;
    } else if (*curr_room_ch == 'D') {
        coords.x = tileSize * (tileNum / 2);
        coords.y = tileSize * (tileNum - 3);
    } else if (*curr_room_ch == 'L') {
        coords.x = tileSize * (tileNum - 3);
        coords.y = tileSize * (tileNum / 2);
    } else if (*curr_room_ch == 'R') {
        coords.x = tileSize * 2;
        coords.y = tileSize * (tileNum / 2);
    }
}

// void Player::Draw(Image& screen, Image& background) {
//     Image player("./resources/player.png");
//
//     if (Moved()) {
//         for (int y = old_coords.y; y <= old_coords.y + tileSize; ++y) {
//             for (int x = old_coords.x; x <= old_coords.x + tileSize; ++x) {
//                 screen.PutPixel(x, y, background.GetPixel(x, y));
//             }
//         }
//         old_coords = coords;
//     }
//     for (int y = coords.y; y <= coords.y + tileSize; ++y) {
//         for (int x = coords.x; x <= coords.x + tileSize; ++x) {
//             Pixel tmp = player_img.GetPixel(x - coords.x, y - coords.y);
//             screen.PutPixel(x, y,
//                                   tmp.a > 0 ? tmp : background.GetPixel(x, y));
//         }
//     }
// }

void Player::Draw(Image &screen, Image &background, Image &player) {
    if (Moved()) {
        for(int y = old_coords.y; y <= old_coords.y + tileSize; ++y) {
            for(int x = old_coords.x; x <= old_coords.x + tileSize; ++x) {
                screen.PutPixel(x, y, background.GetPixel(x, y));
            }
        }
        int bx, by;
        if (coords.y < old_coords.y) {       // go down;
            by = 0;
        } else if (coords.y > old_coords.y) {  // go up;
            by = 3;
        } else if (coords.x < old_coords.x) {  // go left;
            by = 1;
        } else if (coords.x > old_coords.x) {  // go right;
            by = 2;
        }
        if (ctime <= 0.12) {
            bx = 0;
        } else if (ctime > 0.12 && ctime <= 0.24) {
            bx = 1;
        } else {
            bx = 2;
        }
        for (int y = coords.y, m = 0; y <= coords.y + tileSize; ++y, m++) {
            for (int x = coords.x, k = 0; x <= coords.x + tileSize; ++x, k++) {
                Pixel current_pixel = player.GetPixel(bx * tileSize + k, (by + 1) * tileSize - m);
                if (current_pixel.a != 0) {
                    screen.PutPixel(x, y, current_pixel);
                }
            }
        }
        old_coords = coords;
    } else {
        for (int y = coords.y, m = 0; y <= coords.y + tileSize; ++y, m++) {
            for (int x = coords.x, k = 0; x <= coords.x + tileSize; ++x, k++) {
                Pixel current_pixel = player.GetPixel(k, tileSize - m);
                if (current_pixel.a != 0) {
                    screen.PutPixel(x, y, current_pixel);
                }
            }
        }
    }
}

void Player::ctimeInc(float delta) {
    ctime += delta;
    if (ctime > 0.36) {
        ctime = 0;
    }
};
