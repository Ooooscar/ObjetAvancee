#include "LevelEventHandler.hpp"
#include "Level.hpp"

LevelStateContext::LevelStateContext(Level& level)
    : currentState{new LevelStateIdle(level, {-1, -1})}
{}
LevelStateContext::~LevelStateContext() {
    delete currentState;
}
void LevelStateContext::handleMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    currentState = currentState->onMousePositionUpdate(mouseWorldPos);
}
void LevelStateContext::handleMouseClick() {
    currentState = currentState->onMouseClick();
}
void LevelStateContext::handleMouseRelease() {
    currentState = currentState->onMouseRelease();
}


LevelState::LevelState(Level& level, const sf::Vector2i& currentGridPos)
    : level{level}
    , currentGridPos{currentGridPos}
{}
bool LevelState::levelContainsCurrentPos() const {
    return currentGridPos.x != -1;
}
LevelState* LevelState::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    if (level.contains(mouseWorldPos)) {
        currentGridPos_f = level.mapPixelToGrid(mouseWorldPos);
        currentGridPos = sf::Vector2i{currentGridPos_f};
    } else {
        currentGridPos.x = -1;
    }
    return this;
}
LevelState* LevelState::onMouseClick() { return this; }
LevelState* LevelState::onMouseRelease() { return this; }


LevelStateIdle::LevelStateIdle(Level& level, const sf::Vector2i& currentGridPos)
    : LevelState{level, currentGridPos}
{}
LevelState* LevelStateIdle::onMouseClick() {
    if (levelContainsCurrentPos()) {
        int selectedPieceIdx = level.getCurrent(currentGridPos) - 2;
        std::cout << "Clicked on Piece " << selectedPieceIdx << std::endl;
        if (selectedPieceIdx >= 0) {
            return new LevelStateSelected(level, selectedPieceIdx, currentGridPos);
        }
    }
    return this;
}

LevelStateSelected::LevelStateSelected(Level& level, int selectedPieceIdx, const sf::Vector2i& currentGridPos)
    : LevelState{level, currentGridPos}
    , selectedPieceIdx{selectedPieceIdx}
{}
LevelState* LevelStateSelected::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    lastGridPos = currentGridPos;
    LevelState::onMousePositionUpdate(mouseWorldPos);
    if (!levelContainsCurrentPos()) {
        return new LevelStateIdle{level, currentGridPos};
    }
    Direction dir;
    int d = currentGridPos.x - lastGridPos.x;
    if (d == 0) {
        d = currentGridPos.y - lastGridPos.y;
        if (d == 0) {
            return this;
        } else if (d > 0) {
            dir = S;
        } else {
            dir = N;
        }
    } else if (d > 0) {
        dir = E;
    } else {
        dir = W;
    }
    std::cout << "Now start sliding, currentGridPos = (" << currentGridPos.x << ", " << currentGridPos.y << "):\n";
    switch (level.pieces[selectedPieceIdx].onSlide(dir)) {
    case REJECTED:
        return new LevelStateSliding(level, selectedPieceIdx, nullptr, currentGridPos, lastGridPos);
    case ACCEPTED:
        return new LevelStateSliding(level, selectedPieceIdx, new Direction{dir}, currentGridPos, lastGridPos);
    default:
        return this;
    }
}
LevelState* LevelStateSelected::onMouseRelease() {
    level.pieces[selectedPieceIdx].onClick(currentGridPos);
    return new LevelStateIdle{level, currentGridPos};
}

LevelStateSliding::LevelStateSliding(Level& level, int selectedPieceIdx, Direction* direction, const sf::Vector2i& currentGridPos, const sf::Vector2i& startingGridPos)
    : LevelStateSelected{level, selectedPieceIdx, currentGridPos}
    , selectedPieceIdx{selectedPieceIdx}
    , direction{direction}
    , startingGridPos{startingGridPos}
{}
LevelStateSliding::~LevelStateSliding() {
    delete direction;
}
LevelState* LevelStateSliding::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    if (!direction) {
        return LevelStateSelected::onMousePositionUpdate(mouseWorldPos);
    }
    lastGridPos = currentGridPos;
    LevelState::onMousePositionUpdate(mouseWorldPos);
    if (!levelContainsCurrentPos()) {
        return new LevelStateIdle{level, currentGridPos};
    }
    int d;
    switch (*direction) {
    case E:
    case W:
        d = currentGridPos.x - lastGridPos.x;
        if (d == 0) {
            return this;
        } else if (d > 0) {
            *direction = E;
        } else {
            *direction = W;
        }
        break;
    case S:
    case N:
        d = currentGridPos.y - lastGridPos.y;
        if (d == 0) {
            return this;
        } else if (d > 0) {
            *direction = S;
        } else {
            *direction = N;
        }
        break;
    }
    std::cout << "Now sliding, currentGridPos = (" << currentGridPos.x << ", " << currentGridPos.y << "):\n";
    switch (level.pieces[selectedPieceIdx].onSlide(*direction)) {
    case ACCEPTED:
        if (currentGridPos == startingGridPos) {
            return new LevelStateSliding(level, selectedPieceIdx, nullptr, currentGridPos, currentGridPos);
        }
    default:
        break;
    }
    return this;
}
LevelState* LevelStateSliding::onMouseRelease() {
    return new LevelStateIdle(level, currentGridPos);
}

// LevelEventHandler::LevelEventHandler(Level& level)
//     : level{level}
//     , state{IDLE}
//     , currentPieceIdx{-1}
// {}

// void LevelEventHandler::updateMousePosition(const sf::Vector2f& mouseWorldPos)
// {
//     currentWorldPos = mouseWorldPos;
//     currentGridPos_f = level.mapPixelToGrid(mouseWorldPos);
//     currentGridPos = sf::Vector2i{currentGridPos_f};

//     if (state == IDLE) return;

//     int d;
//     Direction dir;
//     switch (state)
//     {
//     case PIECE_SELECTED:
//     case SLIDING_ANY:
//         d = currentGridPos.x - startingGridPos.x;
//         if (d == 0) {
//             d = currentGridPos.y - startingGridPos.y;
//             if (d == 0) {
//                 break;
//             } else {
//                 if (d > 0)
//                     dir = S;
//                 else
//                     dir = N;
//                 switch (level.pieces[currentPieceIdx].onSlide(dir))
//                 {
//                 case INVALID:
//                     break;
//                 case REJECTED:
//                     state = SLIDING_ANY;
//                     break;
//                 case ACCEPTED:
//                     lastGridPos = currentGridPos;
//                     state = SLIDING_VERTICAL;
//                     break;
//                 }
//             }
//         } else {
//             if (d > 0)
//                 dir = E;
//             else
//                 dir = W;
//             switch (level.pieces[currentPieceIdx].onSlide(dir))
//             {
//             case INVALID:
//                 break;
//             case REJECTED:
//                 state = SLIDING_ANY;
//                 break;
//             case ACCEPTED:
//                 lastGridPos = currentGridPos;
//                 state = SLIDING_HORIZONTAL;
//                 break;
//             }
//         }
//         break;

//     case SLIDING_HORIZONTAL:
//         d = currentGridPos.x - lastGridPos.x;
//         if (d == 0)
//             break;
//         if (d > 0)
//             dir = E;
//         else
//             dir = W;
//         if (level.pieces[currentPieceIdx].onSlide(dir)) {
//             lastGridPos = currentGridPos;
//             if (currentGridPos.y == startingGridPos.y)
//                 state = SLIDING_ANY;
//         } else {
//             state = SLIDING_HORIZONTAL;
//         }
//         break;

//     case SLIDING_VERTICAL:
//         d = currentGridPos.y - lastGridPos.y;
//         if (d == 0)
//             break;
//         if (d > 0)
//             dir = S;
//         else
//             dir = N;
//         if (level.pieces[currentPieceIdx].onSlide(dir)) {
//             lastGridPos = currentGridPos;
//             if (currentGridPos.y == startingGridPos.y)
//                 state = SLIDING_ANY;
//         } else {
//             state = SLIDING_VERTICAL;
//         }
//         break;
    
//     default: break;
//     }
// }

// void LevelEventHandler::onMouseClick()
// {
//     if (state == IDLE
//         && level.contains(currentWorldPos))
//     {
//         startingGridPos = currentGridPos;

//         currentPieceIdx = level.getCurrent(currentGridPos) - 2;
//         std::cout << "Clicked on Piece " << currentPieceIdx << std::endl;
//         if (currentPieceIdx >= 0)
//         {
//             state = PIECE_SELECTED;
//         }
//     }
// }

// void LevelEventHandler::onMouseRelease()
// {
//     if (state == PIECE_SELECTED
//         && level.contains(currentWorldPos))
//     {
//         level.pieces[currentPieceIdx].onClick(currentGridPos);
//     }

//     state = IDLE;
// }
