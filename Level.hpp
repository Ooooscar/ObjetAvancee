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
    friend class Level;
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
    const LevelData& originalData;
public:
    std::vector<Piece> pieces;
private:
    std::vector<Operator*> operators;
    // bool inAnimation;

    sf::Vector2f centerCoords;
    float gridSizeInPixels;
public:
    Level(const LevelData& levelData, const sf::Vector2f& centerCoords, float gridSize);
    void restart();

    const sf::Vector2f& getCenterCoords() const;
    float getGridSizeInPixels() const;

    bool contains(const sf::Vector2f& worldPos) const;
    // bool isInAnimation() const;
    bool isFinished() const;

    sf::Vector2f mapPixelToGrid(const sf::Vector2f& worldPos) const;
    sf::Vector2f mapGridToPixel(const sf::Vector2i& gridPos) const;
    sf::Vector2f mapGridToPixel(const sf::Vector2f& gridPos) const;
    void addSquareTo(std::vector<sf::Vertex>& vertexArray, const sf::Vector2i& topLeftGridPos, const sf::Color& color = {});

private:
    Piece* getPieceAtGrid(const sf::Vector2i& gridPos);
    void rebuildMeshBackground();
    void rebuildMeshPiecesAndOperators();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif