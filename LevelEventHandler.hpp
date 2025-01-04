#ifndef _PIECEOUT_LEVEL_EVENT_HANDLER
#define _PIECEOUT_LEVEL_EVENT_HANDLER
#include "Piece.hpp" // pour Direction
#include <SFML/Graphics.hpp>
class Level;
class Button;


class GameState;

class GameStateContext
{
private:
    GameState* currentState;
public:
    GameStateContext(Level* level);
    ~GameStateContext();
    GameStateContext(const GameStateContext&) = delete;
    GameStateContext& operator=(const GameStateContext&) = delete;

    void handleMousePositionUpdate(const sf::Vector2f& mouseWorldPos);
    void handleMouseClick();
    void handleMouseRelease();
};

class GameState
{
protected:
    GameState() = default;
public:
    virtual ~GameState() = default;
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    virtual GameState* onMousePositionUpdate(const sf::Vector2f& mouseWorldPos);
    virtual GameState* onMouseClick();
    virtual GameState* onMouseRelease();
};

class StateMainMenu : public GameState
{
protected:
    std::vector<Button> buttons;
public:
    StateMainMenu();
    GameState* onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) override;
    GameState* onMouseClick() override;
    GameState* onMouseRelease() override;
};

class LevelState : public GameState
{
protected:
    Level* level;
    sf::Vector2f currentGridPos_f;
    sf::Vector2i currentGridPos;
    virtual ~LevelState();
    bool levelContainsCurrentPos() const;
public:
    LevelState(Level* level, const sf::Vector2i& currentGridPos);
    GameState* onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) override;
};

class LevelStateIdle : public LevelState
{
public:
    LevelStateIdle(Level* level, const sf::Vector2i& currentGridPos);
    GameState* onMouseClick() override;
};

class LevelStatePieceSelected : public LevelState
{
protected:
    int selectedPieceIdx;
    sf::Vector2i lastGridPos;
public:
    LevelStatePieceSelected(Level* level, int selectedPieceIdx, const sf::Vector2i& currentGridPos);
    GameState* onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) override;
    GameState* onMouseRelease() override;
};

class LevelStatePieceSliding : public LevelStatePieceSelected
{
protected:
    Direction* direction;
    sf::Vector2i startingGridPos;
    virtual ~LevelStatePieceSliding();
public:
    LevelStatePieceSliding(Level* level, int selectedPieceIdx, Direction* direction, const sf::Vector2i& currentGridPos, const sf::Vector2i& startingGridPos);
    GameState* onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) override;
    GameState* onMouseRelease() override;
};

#endif