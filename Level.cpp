#include "Level.hpp"
#include <iostream>

LevelData::LevelData( const int nCol, const int nRow,
            std::vector<int>&& gridFinal, std::vector<int>&& gridCurrent,
            std::initializer_list<OperatorData> dataOfOperators,
            std::initializer_list<PieceColor> colors )
    : nCol{nCol}
    , nRow{nRow}
    , gridFinal{gridFinal}
    , gridCurrent{gridCurrent}
    , dataOfOperators{dataOfOperators}
    , colors{colors}
{}

const int LevelData::getFinal(const sf::Vector2i& gridPos) const
{
    return gridFinal[gridPos.y * nCol + gridPos.x];
}
const int LevelData::getCurrent(const sf::Vector2i& gridPos) const
{
    return gridCurrent[gridPos.y * nCol + gridPos.x];
}
void LevelData::setCurrent(const sf::Vector2i& gridPos, int value)
{
    gridCurrent[gridPos.y * nCol + gridPos.x] = value;
}

Level::Level(const LevelData &levelData, const sf::Vector2f& centerCoords, float gridSizeInPixels)
    : LevelData{levelData}
    , DrawableShape{{}, sf::Triangles}
    , pieces{}
    , operators{}
    , inAnimation{false}
    , finished{false}
    , centerCoords{centerCoords}
    , gridSizeInPixels{gridSizeInPixels}
    , movingPiece{nullptr}
{
    pieces.reserve(colors.size());
    operators.reserve(dataOfOperators.size());

    int nbFloorTiles = 0;
    for (int tileData : gridFinal) {
        if (tileData == 0) ++nbFloorTiles;
    }
    vertexArray.reserve((nbFloorTiles + 1) * 6);

    float levelW = nCol * gridSizeInPixels;
    float levelH = nRow * gridSizeInPixels;
    float x0 = centerCoords.x - levelW / 2;
    float x1 = centerCoords.x + levelW / 2;
    float y0 = centerCoords.y - levelH / 2;
    float y1 = centerCoords.y + levelH / 2;
    // panneau central
    vertexArray.emplace_back(sf::Vertex{{x0, y0}, PieceColor::WALL.backgroundColor});
    vertexArray.emplace_back(sf::Vertex{{x1, y0}, PieceColor::WALL.backgroundColor});
    vertexArray.emplace_back(sf::Vertex{{x0, y1}, PieceColor::WALL.backgroundColor});
    vertexArray.emplace_back(sf::Vertex{{x1, y0}, PieceColor::WALL.backgroundColor});
    vertexArray.emplace_back(sf::Vertex{{x0, y1}, PieceColor::WALL.backgroundColor});
    vertexArray.emplace_back(sf::Vertex{{x1, y1}, PieceColor::WALL.backgroundColor});

    // lecture des données des cases du niveau
	for (int pieceIdx = 0; pieceIdx < static_cast<int>(colors.size()); ++pieceIdx) {
		pieces.emplace_back(Piece{{{}, colors[pieceIdx]}, *this, pieceIdx});
	}
    int cellIdx = 0;
	for (int y = 0; y < nRow; ++y) {
		for (int x = 0; x < nCol; ++x) {
			int cellData = gridFinal[cellIdx];
			switch (cellData) {
			case 0 :
				break;
			case 1 :
				addSquareTo(vertexArray, {x, y}, PieceColor::WALL.primaryColor);
				break;
			default	:
				int pieceIdx = cellData - 2;
				// if (pieceIdx < static_cast<int>(pieces.size()))
				addSquareTo(vertexArray, {x, y}, colors[pieceIdx].backgroundColor);
                pieces[pieceIdx].setHasCorrectPosition();
				break;
			}
			int startingCellData = gridCurrent[cellIdx];
			if (startingCellData >= 2) {
			    int pieceIdx = startingCellData - 2;
				// if (pieceIdx < static_cast<int>(pieces.size())) {
				    pieces[pieceIdx].emplaceCell({x, y});
                // } else {
                //     std::cout << pieceIdx << std::endl;
                // }
			}
			++cellIdx;
		}
	}
    for (OperatorData& opData : dataOfOperators)
    {
        int pieceIdx = getCurrent(opData.gridPos) - 2;
        if (pieceIdx >= 0)
        {
            operators.emplace_back(*this, pieces[pieceIdx], opData);
            pieces[pieceIdx].addOperator(operators.back());
        }
    }

    for (Piece& piece : pieces)
    {
        piece.update();
    }
}

bool Level::isInAnimation() const
{
    // return movingPiece;
    return false; // TODO
}

const Operator& Level::getOperatorAt(int operatorIdx) const { return operators[operatorIdx]; }
const sf::Vector2f& Level::getCenterCoords() const { return centerCoords; }
float Level::getGridSizeInPixels() const { return gridSizeInPixels; }

bool Level::contains(const sf::Vector2f& worldPos) const
{
    return vertexArray[0].position.x <= worldPos.x && vertexArray[1].position.x >= worldPos.x
        && vertexArray[0].position.y <= worldPos.y && vertexArray[2].position.y >= worldPos.y;
}

sf::Vector2f Level::mapPixelToGrid(const sf::Vector2f& worldPos) const
{
	return { (worldPos.x - centerCoords.x) / gridSizeInPixels + nCol * 0.5f,
		     (worldPos.y - centerCoords.y) / gridSizeInPixels + nRow * 0.5f};
}
sf::Vector2f Level::mapGridToPixel(const sf::Vector2i& gridPos) const
{
    return { (gridPos.x - nCol * 0.5f) * gridSizeInPixels + centerCoords.x,
		     (gridPos.y - nRow * 0.5f) * gridSizeInPixels + centerCoords.y };
}
sf::Vector2f Level::mapGridToPixel(const sf::Vector2f& gridPos) const
{
    return { (gridPos.x - nCol * 0.5f) * gridSizeInPixels + centerCoords.x,
		     (gridPos.y - nRow * 0.5f) * gridSizeInPixels + centerCoords.y };
}
void Level::addSquareTo(std::vector<sf::Vertex>& vertexArray, const sf::Vector2i& topLeftGridPos, const sf::Color& color)
{
    sf::Vector2f topLeftPos{mapGridToPixel(topLeftGridPos)};
    float x0 = topLeftPos.x;
    float x1 = x0 + gridSizeInPixels;
    float y0 = topLeftPos.y;
    float y1 = y0 + gridSizeInPixels;
    vertexArray.emplace_back(sf::Vertex{{x0, y0}, color});
    vertexArray.emplace_back(sf::Vertex{{x1, y0}, color});
    vertexArray.emplace_back(sf::Vertex{{x0, y1}, color});
    vertexArray.emplace_back(sf::Vertex{{x1, y0}, color});
    vertexArray.emplace_back(sf::Vertex{{x0, y1}, color});
    vertexArray.emplace_back(sf::Vertex{{x1, y1}, color});
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    DrawableShape::draw(target, states);
    for (const Piece& piece : pieces)
        target.draw(piece);
    for (const Operator& op : operators)
        target.draw(op);
}