#include "direction.h"

Direction_e getReflectDirection(Direction_e currDirection)
{
    Direction_e invDirection = DIRECTION_MAX;

    switch (currDirection)
    {
    case DIRECTION_UP:
        invDirection = DIRECTION_DOWN;
        break;
    case DIRECTION_DOWN:
        invDirection = DIRECTION_UP;
        break;
    case DIRECTION_LEFT:
        invDirection = DIRECTION_RIGHT;
        break;
    case DIRECTION_RIGHT:
        invDirection = DIRECTION_LEFT;
        break;
    case DIRECTION_UP_LEFT:
        invDirection = DIRECTION_DOWN_RIGHT;
        break;
    case DIRECTION_DOWN_RIGHT:
        invDirection = DIRECTION_UP_LEFT;
        break;
    case DIRECTION_UP_RIGHT:
        invDirection = DIRECTION_DOWN_LEFT;
        break;
    case DIRECTION_DOWN_LEFT:
        invDirection = DIRECTION_UP_RIGHT;
        break;
    default:
        break;
    }

    return invDirection;
}

Direction_e getRefractionDirection(Direction_e currDirection)
{
    Direction_e invDirection = DIRECTION_MAX;

    switch (currDirection)
    {
    case DIRECTION_UP:
        invDirection = DIRECTION_DOWN;
        break;
    case DIRECTION_DOWN:
        invDirection = DIRECTION_UP;
        break;
    case DIRECTION_LEFT:
        invDirection = DIRECTION_RIGHT;
        break;
    case DIRECTION_RIGHT:
        invDirection = DIRECTION_LEFT;
        break;
    case DIRECTION_UP_LEFT:
        invDirection = DIRECTION_DOWN_LEFT;
        break;
    case DIRECTION_DOWN_RIGHT:
        invDirection = DIRECTION_UP_RIGHT;
        break;
    case DIRECTION_UP_RIGHT:
        invDirection = DIRECTION_DOWN_RIGHT;
        break;
    case DIRECTION_DOWN_LEFT:
        invDirection = DIRECTION_UP_LEFT;
        break;
    default:
        break;
    }

    return invDirection;
}

