#ifndef _PIECEOUT_STATE_MACHINE
#define _PIECEOUT_STATE_MACHINE
#include "Piece.hpp" // pour Direction
#include <SFML/Graphics.hpp>
class Button;
class Level;


class GameState;

class GameStateMachine
{
private:
    static GameStateMachine* context;
    GameStateMachine(sf::RenderWindow& window);
public:
    static void initializeStateMachine(sf::RenderWindow& window);
    static GameStateMachine& getContext();
    ~GameStateMachine();
    GameStateMachine(const GameStateMachine&) = delete;
    GameStateMachine& operator=(const GameStateMachine&) = delete;

private:
    GameState* currentState;
public:
    sf::RenderWindow& window;
    void tick();
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
    virtual void render(sf::RenderWindow& window) const;
};

class MainMenuState : public GameState
{
protected:
    std::vector<Button*> buttons;
public:
    MainMenuState();
    ~MainMenuState();
    GameState* onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) override;
    GameState* onMouseClick() override;
    GameState* onMouseRelease() override;
    void render(sf::RenderWindow& window) const override;
};

class LevelState : public GameState
{
protected:
    Level* level;
    sf::Vector2f currentGridPos_f;
    sf::Vector2i currentGridPos;
    virtual ~LevelState();
    bool levelContainsCurrentPos() const;
    LevelState(Level* level, const sf::Vector2i& currentGridPos);
            // on veut interdire la construction d'un `LevelState` abstrait
public:
    GameState* onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) override;
    void render(sf::RenderWindow& window) const override;
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