#ifndef _PIECEOUT_LEVEL_STATE
#define _PIECEOUT_LEVEL_STATE
#include <SFML/Graphics.hpp>

class LevelState
{
public:
    virtual void onMouseMove() = 0;
    virtual void onMouseClick() = 0;
};

class IdleState

#endif