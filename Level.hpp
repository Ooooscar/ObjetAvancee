#ifndef _PIECEOUT_LEVEL
#define _PIECEOUT_LEVEL
#include "Piece.hpp"
// #include "Operator.hpp"
#include "DrawableShape.hpp"
#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>

class LevelData
{
protected:
    int nCol;
    int nRow;
    std::vector<int> gridFinal;
    std::vector<int> gridCurrent;
    std::vector<OperatorData> dataOfOperators;
    std::vector<PieceColor> colors;
public:
    LevelData( const int nCol, const int nRow,
            std::vector<int>&& gridFinal, std::vector<int>&& gridCurrent,
            std::initializer_list<OperatorData> dataOfOperators,
            std::initializer_list<PieceColor> colors );
    const int getFinal(const sf::Vector2i& gridPos) const;
    const int getCurrent(const sf::Vector2i& gridPos) const;
    void setCurrent(const sf::Vector2i& gridPos, int value);
};

class Level : public LevelData, public DrawableShape
{
private:
public:
    std::vector<Piece> pieces;
private:
    std::vector<Operator> operators;
    bool inAnimation;
    bool finished;

    sf::Vector2f centerCoords;
    float gridSizeInPixels;

    Piece* movingPiece;

    // friend class LevelEventHandler;
public:
    Level(const LevelData& levelData, const sf::Vector2f& centerCoords, float gridSize);

    // les operateurs du niveau seront supprimés par le déstructeur des pièces
    virtual ~Level() = default;
    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;
    Level(Level&&) = delete;
    Level& operator=(Level&&) = delete;

    const Operator& getOperatorAt(int operatorIdx) const;

    bool contains(const sf::Vector2f& worldPos) const;
    const sf::Vector2f& getCenterCoords() const;
    float getGridSizeInPixels() const;
    bool isInAnimation() const;

    sf::Vector2f mapPixelToGrid(const sf::Vector2f& worldPos) const;
    sf::Vector2f mapGridToPixel(const sf::Vector2i& gridPos) const;
    sf::Vector2f mapGridToPixel(const sf::Vector2f& gridPos) const;
    void addSquareTo(std::vector<sf::Vertex>& vertexArray, const sf::Vector2i& topLeftGridPos, const sf::Color& color = {});

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif