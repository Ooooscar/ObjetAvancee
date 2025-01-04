#include "LevelEventHandler.hpp"
#include "Level.hpp"

GameStateContext::GameStateContext(Level* level)
    : currentState{new LevelStateIdle(level, {-1, -1})}
{}
GameStateContext::~GameStateContext() {
    delete currentState;
}
void GameStateContext::handleMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    currentState = currentState->onMousePositionUpdate(mouseWorldPos);
}
void GameStateContext::handleMouseClick() {
    currentState = currentState->onMouseClick();
}
void GameStateContext::handleMouseRelease() {
    currentState = currentState->onMouseRelease();
}
GameState* GameState::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) { return this; }
GameState* GameState::onMouseClick() { return this; }
GameState* GameState::onMouseRelease() { return this; }

// StateMainMenu()


LevelState::LevelState(Level* level, const sf::Vector2i& currentGridPos)
    : GameState{}
    , level{level}
    , currentGridPos{currentGridPos}
{}
LevelState::~LevelState() {
    delete level;
}

bool LevelState::levelContainsCurrentPos() const {
    return currentGridPos.x != -1;
}
GameState* LevelState::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    if (level->contains(mouseWorldPos)) {
        currentGridPos_f = level->mapPixelToGrid(mouseWorldPos);
        currentGridPos = sf::Vector2i{currentGridPos_f};
    } else {
        currentGridPos.x = -1;
    }
    return this;
}


LevelStateIdle::LevelStateIdle(Level* level, const sf::Vector2i& currentGridPos)
    : LevelState{level, currentGridPos}
{}
GameState* LevelStateIdle::onMouseClick() {
    if (levelContainsCurrentPos()) {
        int selectedPieceIdx = level->getCurrent(currentGridPos) - 2;
        std::cout << "Clicked on Piece " << selectedPieceIdx << std::endl;
        if (selectedPieceIdx >= 0) {
            return new LevelStatePieceSelected(level, selectedPieceIdx, currentGridPos);
        }
    }
    return this;
}

LevelStatePieceSelected::LevelStatePieceSelected(Level* level, int selectedPieceIdx, const sf::Vector2i& currentGridPos)
    : LevelState{level, currentGridPos}
    , selectedPieceIdx{selectedPieceIdx}
{}
GameState* LevelStatePieceSelected::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
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
    switch (level->pieces[selectedPieceIdx].onMouseSlide(dir)) {
    case REJECTED:
        return new LevelStatePieceSliding(level, selectedPieceIdx, nullptr, currentGridPos, lastGridPos);
    case ACCEPTED:
        return new LevelStatePieceSliding(level, selectedPieceIdx, new Direction{dir}, currentGridPos, lastGridPos);
    default:
        return this;
    }
}
GameState* LevelStatePieceSelected::onMouseRelease() {
    level->pieces[selectedPieceIdx].onMouseActivate(currentGridPos);
    return new LevelStateIdle{level, currentGridPos};
}

LevelStatePieceSliding::LevelStatePieceSliding(Level* level, int selectedPieceIdx, Direction* direction, const sf::Vector2i& currentGridPos, const sf::Vector2i& startingGridPos)
    : LevelStatePieceSelected{level, selectedPieceIdx, currentGridPos}
    , direction{direction}
    , startingGridPos{startingGridPos}
{}
LevelStatePieceSliding::~LevelStatePieceSliding() {
    delete direction;
}
GameState* LevelStatePieceSliding::onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) {
    if (!direction) {
        return LevelStatePieceSelected::onMousePositionUpdate(mouseWorldPos);
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
    switch (level->pieces[selectedPieceIdx].onMouseSlide(*direction)) {
    case ACCEPTED:
        if (currentGridPos == startingGridPos) {
            return new LevelStatePieceSliding(level, selectedPieceIdx, nullptr, currentGridPos, currentGridPos);
        }
    default:
        break;
    }
    return this;
}
GameState* LevelStatePieceSliding::onMouseRelease() {
    return new LevelStateIdle(level, currentGridPos);
}
