#ifndef _PIECEOUT_PIECE
#define _PIECEOUT_PIECE
#include "Operator.hpp"
#include "DrawableShape.hpp"
#include <SFML/Graphics.hpp>
#include <array>

enum Direction
{
    E = 0,
    S = 1,
    W = 2,
    N = 3,
};

enum ActionResponse
{
    INVALID,
    REJECTED,
    ACCEPTED,
};

class PieceColor
{
public:
    sf::Color primaryColor;
    sf::Color backgroundColor;
    PieceColor(const sf::Color& primaryColor, const sf::Color& backgroundColor);

    static const PieceColor WALL;
    static const PieceColor RED;
    static const PieceColor ORANGE;
    static const PieceColor GREEN;
    static const PieceColor CYAN;
    static const PieceColor BLUE;
    static const PieceColor PURPLE;
};

class PieceData
{
protected:
	std::vector<sf::Vector2i> gridCoords;
	PieceColor color;
public:
	PieceData(const std::vector<sf::Vector2i>& gridCoords, const PieceColor& color);

    void emplaceCell(const sf::Vector2i& gridPos);
};

class Level;

class Piece : public PieceData, public DrawableShape
{
private:
    Level& level;
    const int pieceIdx;

    std::array<MovementOperator*, 4> movementOperators;
	std::vector<Operator*> mainOperators;

    // sf::Clock* animationTimer;
    bool hasCorrectPosition;
    bool atCorrectPosition;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void acceptTransform(const sf::Transform& transform) override;
public:
    Piece(const PieceData& data, Level& level, int pieceIdx);

    virtual ~Piece() = default;
    Piece(const Piece&) = delete;
    Piece& operator=(const Piece&) = delete;
    Piece(Piece&&) = default; // comme le constructeur de déplacement est défini,
                              // on peut mettre les `Piece`s dans un `std::vector`
    Piece& operator=(Piece&&) = delete;

    const Level& getLevel() const;
    const int getIndex() const;
    const bool isAtCorrectPosition() const;
    // const bool isInAnimation() const;
    const bool canMoveInDirection(Direction direction) const;

    bool addOperator(Operator& op);
    void setHasCorrectPosition();

    // void transformWorldCoords(const sf::Transform& transform);
    // void move(int direction, float progress);
    // void rotate(const sf::Vector2f& originWorldPos, float angleInRad);
    // void flipVertical(const sf::Vector2f& originWorldPos, float progress);
    // void flipHorizontal(const sf::Vector2f& originWorldPos, float progress);

    ActionResponse onMouseSlide(Direction direction);
    ActionResponse onMouseActivate(const sf::Vector2i& gridPos);
    ActionResponse trigger(Operator& op);
    void accept(Operator& op);
    void reject(Operator& op);

    void rebuildMesh();
    // void initializeAnimation();
    // void endAnimation();
};

#endif