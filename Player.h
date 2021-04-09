#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"

struct Point {
  int x;
  int y;
};

enum class MovementDir {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

struct Player {
    explicit Player(Point pos = {.x = 400, .y = 400}) : coords(pos),
                                                          old_coords(coords) {
    };

    bool Moved() const;
    void ProcessInput(MovementDir dir, char room_buff[25][25], char *curr_room_ch, bool *trap_activated,
                                                                    bool *win, bool *lose);
    void Draw(Image &screen, Image& background, Image& player);
    void changePos(char *curr_room_ch);
    void ctimeInc(float delta);
private:
    float ctime = 0;
    Point coords {.x = 10, .y = 10};
    Point old_coords {.x = 10, .y = 10};
    Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
    int move_speed = 1;
};

#endif //MAIN_PLAYER_H
