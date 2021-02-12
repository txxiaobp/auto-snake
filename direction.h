#ifndef DIRECTION_H
#define DIRECTION_H

const int ROUTE_METHOD_DIRECTION_COUNT = 4;

typedef enum
{
    DIRECTION_UP         = 0,
    DIRECTION_DOWN       = 1,
    DIRECTION_LEFT       = 2,
    DIRECTION_RIGHT      = 3,
    DIRECTION_UP_LEFT    = 4,
    DIRECTION_UP_RIGHT   = 5,
    DIRECTION_DOWN_LEFT  = 6,
    DIRECTION_DOWN_RIGHT = 7,
    DIRECTION_MAX
}Direction_e;


const int DIRECTION_COUNT = 8;
const int directions[DIRECTION_COUNT][2] = {

    {0,   1},  //up
    {0,  -1},  //down
    {-1,  0},  //left
    {1,   0},  //right
    {-1,  1},  //up-left
    {1,   1},  //up-right
    {-1, -1},  //down-left
    {1,  -1},  //down-right
};



Direction_e getReflectDirection(Direction_e currDirection);
Direction_e getRefractionDirection(Direction_e currDirection);

#endif // DIRECTION_H
