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

Level::Level(const LevelData& levelData, const sf::Vector2f& centerCoords, float gridSizeInPixels)
    : LevelData{levelData} // ceci est une copie
    , DrawableShape{{}, sf::Triangles}
    , originalData{levelData} // ceci est une référence const vers levelData
    , pieces{}
    , operators{}
    // , inAnimation{false}
    , centerCoords{centerCoords}
    , gridSizeInPixels{gridSizeInPixels}
{
    pieces.reserve(colors.size());
    operators.reserve(dataOfOperators.size());

    int nbFloorTiles = 0;
    for (int tileData : gridFinal) {
        if (tileData == 0) ++nbFloorTiles;
    }
    vertexArray.reserve((nbFloorTiles + 1) * 6);

    rebuildMeshBackground();
    rebuildMeshPiecesAndOperators();
}

void Level::rebuildMeshBackground()
{
    vertexArray.clear();
    float levelW = nCol * gridSizeInPixels;
    float levelH = nRow * gridSizeInPixels;
    float x0 = centerCoords.x - levelW / 2;
    float x1 = centerCoords.x + levelW / 2;
    float y0 = centerCoords.y - levelH / 2;
    float y1 = centerCoords.y + levelH / 2;
    vertexArray.emplace_back(sf::Vertex{{x0, y0}, PieceColor::WALL.backgroundColor});
    vertexArray.emplace_back(sf::Vertex{{x1, y0}, PieceColor::WALL.backgroundColor});
    vertexArray.emplace_back(sf::Vertex{{x0, y1}, PieceColor::WALL.backgroundColor});
    vertexArray.emplace_back(sf::Vertex{{x1, y0}, PieceColor::WALL.backgroundColor});
    vertexArray.emplace_back(sf::Vertex{{x0, y1}, PieceColor::WALL.backgroundColor});
    vertexArray.emplace_back(sf::Vertex{{x1, y1}, PieceColor::WALL.backgroundColor});

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
				addSquareTo(vertexArray, {x, y}, colors[pieceIdx].backgroundColor);
				break;
			}
			++cellIdx;
		}
	}
}

void Level::rebuildMeshPiecesAndOperators()
{
    pieces.clear();
    operators.clear();

	for (int pieceIdx = 0; pieceIdx < static_cast<int>(colors.size()); ++pieceIdx) {
		pieces.emplace_back(Piece{{{}, colors[pieceIdx]}, *this, pieceIdx});
	}
    int cellIdx = 0;
	for (int y = 0; y < nRow; ++y) {
		for (int x = 0; x < nCol; ++x) {
			int pieceIdxFin = gridFinal[cellIdx] - 2;
            if (pieceIdxFin >= 0)
                pieces[pieceIdxFin].setHasCorrectPosition();
			int pieceIdxCur = gridCurrent[cellIdx] - 2;
			if (pieceIdxCur >= 0) {
                pieces[pieceIdxCur].emplaceCell({x, y});
			}
			++cellIdx;
		}
	}
    OperatorFactory opFactory{*this};
    for (OperatorData& opData : dataOfOperators) {
        Piece* source = getPieceAtGrid(opData.gridPos);
        if (!source)
            throw std::runtime_error("Position d'opérateur invalide !");
        Operator* op = opFactory.createOperator(opData, *source);
        
                // la traitement des erreurs est fait dans `OperatorFactory`
        operators.emplace_back(std::move(op));
        source->addOperator(*operators.back());
    }

    for (Piece& piece : pieces) {
        piece.rebuildMesh(); // dans `Piece::rebuildMesh()`, les `Operator::rebuildMesh()` sont appelées
    }
}

void Level::restart()
{
    gridCurrent = originalData.gridCurrent;
    dataOfOperators = originalData.dataOfOperators;
    rebuildMeshPiecesAndOperators();
}

// bool Level::isInAnimation() const
// {
//     return false; // TODO
// }

const sf::Vector2f& Level::getCenterCoords() const { return centerCoords; }
float Level::getGridSizeInPixels() const { return gridSizeInPixels; }

bool Level::contains(const sf::Vector2f& worldPos) const
{
    return vertexArray[0].position.x <= worldPos.x && vertexArray[1].position.x >= worldPos.x
        && vertexArray[0].position.y <= worldPos.y && vertexArray[2].position.y >= worldPos.y;
}

bool Level::isFinished() const
{
    for (const Piece& p : pieces) {
        if (!p.isAtCorrectPosition()) {
            return false;
        }
    }
    return true;
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

Piece* Level::getPieceAtGrid(const sf::Vector2i& gridPos) {
    int pieceIdx = getCurrent(gridPos) - 2;
    return (pieceIdx < 0) ? nullptr : &pieces[pieceIdx];
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
    for (const Operator* op : operators)
        target.draw(*op);
}