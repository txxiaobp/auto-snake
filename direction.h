#ifndef DIRECTION_H
#define DIRECTION_H


typedef enum
{
    DIRECTION_UP     = 0,
    DIRECTION_DOWN   = 1,
    DIRECTION_LEFT   = 2,
    DIRECTION_RIGHT  = 3,
    DIRECTION_MAX
}Direction_e;


#define FOUR_DIRECTIONS

#ifdef FOUR_DIRECTIONS
const int DIRECTION_COUNT = 4;
const int directions[DIRECTION_COUNT][2] = {
    {1, 0}, //right
    {0, 1}, //up
    {-1, 0}, //left
    {0, -1}, //down
};
#endif


#ifdef EIGHT_DIRECTIONS
const int DIRECTION_COUNT = 8;
const int directions[DIRECTION_COUNT][2] = {
    {1, 0}, //right
    {1, 1}, //right-up
    {0, 1}, //up
    {-1, 1}, //left-up
    {-1, 0}, //left
    {-1, -1}, //left-down
    {0, -1}, //down
    {1, -1}, //right-down
};
#endif

#endif // DIRECTION_H
