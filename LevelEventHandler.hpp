#ifndef _PIECEOUT_LEVEL_EVENT_HANDLER
#define _PIECEOUT_LEVEL_EVENT_HANDLER
#include "Piece.hpp" // pour Direction
#include <SFML/Graphics.hpp>

class Level;

class LevelState
{
protected:
    Level& level;
    sf::Vector2f currentGridPos_f;
    sf::Vector2i currentGridPos;
    LevelState(Level& level, const sf::Vector2i& currentGridPos);
    bool levelContainsCurrentPos() const;
public:
    virtual ~LevelState() = default;
    LevelState(const LevelState&) = delete;
    LevelState& operator=(const LevelState&) = delete;

    virtual LevelState* onMousePositionUpdate(const sf::Vector2f& mouseWorldPos);
    virtual LevelState* onMouseClick();
    virtual LevelState* onMouseRelease();
};

class LevelStateContext
{
private:
    LevelState* currentState;
public:
    LevelStateContext(Level& level);
    ~LevelStateContext();
    LevelStateContext(const Level&) = delete;
    LevelStateContext& operator=(const LevelStateContext&) = delete;

    void handleMousePositionUpdate(const sf::Vector2f& mouseWorldPos);
    void handleMouseClick();
    void handleMouseRelease();
};

class LevelStateIdle : public LevelState
{
public:
    LevelStateIdle(Level& level, const sf::Vector2i& currentGridPos);
    LevelState* onMouseClick() override;
};

class LevelStateSelected : public LevelState
{
private:
    int selectedPieceIdx;
    sf::Vector2i lastGridPos;
public:
    LevelStateSelected(Level& level, int selectedPieceIdx, const sf::Vector2i& currentGridPos);
    LevelState* onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) override;
    LevelState* onMouseRelease() override;
};

class LevelStateSliding : public LevelStateSelected
{
private:
    int selectedPieceIdx;
    Direction* direction;
    sf::Vector2i lastGridPos, startingGridPos;
public:
    virtual ~LevelStateSliding();
    LevelStateSliding(Level& level, int selectedPieceIdx, Direction* direction, const sf::Vector2i& currentGridPos, const sf::Vector2i& startingGridPos);
    LevelState* onMousePositionUpdate(const sf::Vector2f& mouseWorldPos) override;
    LevelState* onMouseRelease() override;
};

// enum LevelControlState
// {
//     IDLE,
//     PIECE_SELECTED,
//     SLIDING_VERTICAL,
//     SLIDING_HORIZONTAL,
//     SLIDING_ANY,
// };

// class LevelEventHandler
// {
// private:
//     Level& level;
//     LevelControlState state;
//     sf::Vector2f currentWorldPos;
//     sf::Vector2f currentGridPos_f;
//     sf::Vector2i currentGridPos, lastGridPos, startingGridPos;
//     int selectedPieceIdx;
// public:
//     LevelEventHandler(Level& level);
//     void updateMousePosition(const sf::Vector2f& mouseWorldPos);
//     void onMouseClick();
//     void onMouseRelease();
// };

#endif