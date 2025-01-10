#include "Operator.hpp"
#include "Piece.hpp"
#include "Level.hpp"
#define _USE_MATH_DEFINES //pour M_PI
#include <array>
#include <cmath>

const std::array<const std::vector<sf::Vector2f>, OperatorType::Count> vertexPatterns {
    // movement east
    std::vector<sf::Vector2f>{{0.88f, 0.5f}, {0.7f, 0.36f}, {0.7f, 0.64f}},
    // movement south
    std::vector<sf::Vector2f>{{0.5f, 0.88f}, {0.36f, 0.7f}, {0.64f, 0.7f}},
    // movement west
    std::vector<sf::Vector2f>{{0.12f, 0.5f}, {0.3f, 0.36f}, {0.3f, 0.64f}},
    // movement north
    std::vector<sf::Vector2f>{{0.5f, 0.12f}, {0.36f, 0.3f}, {0.64f, 0.3f}},
    // rotation clockwise
    std::vector<sf::Vector2f>{{0.25f, 0.25f}, {0.25f, 0.75f}, {0.75f, 0.25f}, {0.75f, 0.75f}},
    // rotation counterclockwise
    std::vector<sf::Vector2f>{{0.18f, 0.5f}, {0.5f, 0.18f}, {0.5f, 0.82f}, {0.82f, 0.5f}},
    // flip horizontal
    std::vector<sf::Vector2f>{{0.45f, 0.2f}, {0.55f, 0.2f}, {0.45f, 0.8f}, {0.55f, 0.8f}},
    // flip vertical
    std::vector<sf::Vector2f>{{0.2f, 0.45f}, {0.2f, 0.55f}, {0.8f, 0.45f}, {0.8f, 0.55f}},
};

OperatorData::OperatorData(OperatorType type, const sf::Vector2i& gridPos)
    : type{type}
    , gridPos{gridPos}
{}

OperatorFactory::OperatorFactory(const Level& level)
    : level{level}
{}

Operator* OperatorFactory::createOperator(const OperatorData& data, Piece& source) const {
    switch (data.type) {
        case MOV_E:
        case MOV_S:
        case MOV_W:
        case MOV_N:
            return new MovementOperator(data, source, level.mapGridToPixel(data.gridPos));
            break;
        case ROT_CW:
        case ROT_CCW:
            return new RotationOperator(data, source, level.mapGridToPixel(data.gridPos));
        case FLP_HOR:
        case FLP_VRT:
            return new FlipOperator(data, source, level.mapGridToPixel(data.gridPos));
        default:
            throw std::runtime_error("Type d'opérateur invalide !");
    }
}

Operator::Operator(const OperatorData& data, Piece& source, sf::Vector2f&& worldPos)
    : OperatorData{data} // on veut faire une copie ici
    , DrawableShape{std::vector<sf::Vertex>(vertexPatterns[type].size()), sf::TriangleStrip}
    , source{source}
    , worldPos{std::move(worldPos)}
{}

OperatorType Operator::getType() const { return type; }
const sf::Vector2i& Operator::getGridPosition() const { return gridPos; }

void Operator::setGridPosition(const sf::Vector2i& otherGridPos) {
    gridPos = otherGridPos;
    worldPos = source.getLevel().mapGridToPixel(gridPos);
}

void Operator::rebuildMesh() {
    float gridSizeInPixels = source.getLevel().getGridSizeInPixels();
    vertexArray.clear();
    for (const sf::Vector2f& v : vertexPatterns[type])
        vertexArray.emplace_back(source.getLevel().mapGridToPixel(gridPos) + gridSizeInPixels * v);
}

void Operator::mapGridPosInplace(sf::Vector2i& otherGridPos) const
{
    switch (type)
    {
    case MOV_E: ++otherGridPos.x; break;
    case MOV_S: ++otherGridPos.y; break;
    case MOV_W: --otherGridPos.x; break;
    case MOV_N: --otherGridPos.y; break;
    case ROT_CW:
        {   // Rotation 90° horarire : (x, y) -> (y, -x)
            int xTmp = gridPos.x - (otherGridPos.y - gridPos.y);
            otherGridPos.y = gridPos.y + (otherGridPos.x - gridPos.x);
            otherGridPos.x = xTmp;
        }
        break;
    case ROT_CCW:
        {   // Rotation 90° anti-horarire : (x, y) -> (-y, x)
            int xTmp = gridPos.x + (otherGridPos.y - gridPos.y);
            otherGridPos.y = gridPos.y - (otherGridPos.x - gridPos.x);
            otherGridPos.x = xTmp;
        }
        break;
    case FLP_HOR:
        {   // Réflexion par rapport à l'axe des y : (x, y) -> (-x, y)
            otherGridPos.x = 2*gridPos.x - otherGridPos.x;
        }
        break;
    case FLP_VRT:
        {   // Réflexion par rapport à l'axe des x : (x, y) -> (x, -y)
            otherGridPos.y = 2*gridPos.y - otherGridPos.y;
        }
        break;
    default:
        break;
    }
}

void MovementOperator::mapGridPosInplace(sf::Vector2i& otherGridPos) const {
    switch (type) {
        case MOV_E: ++otherGridPos.x; break;
        case MOV_S: ++otherGridPos.y; break;
        case MOV_W: --otherGridPos.x; break;
        case MOV_N: --otherGridPos.y; break;
        default: throw std::runtime_error("Orientation de mouvement invalide !");
    }
}
void MovementOperator::mapOperatorTypeInplace(OperatorType& otherType) const {
    // rien à faire
}

void RotationOperator::mapGridPosInplace(sf::Vector2i& otherGridPos) const {
    switch (type) {
        case ROT_CW:
            {   // Rotation 90° horarire : (x, y) -> (y, -x)
                int xTmp = gridPos.x - (otherGridPos.y - gridPos.y);
                otherGridPos.y = gridPos.y + (otherGridPos.x - gridPos.x);
                otherGridPos.x = xTmp;
            }
            break;
        case ROT_CCW:
            {   // Rotation 90° anti-horarire : (x, y) -> (-y, x)
                int xTmp = gridPos.x + (otherGridPos.y - gridPos.y);
                otherGridPos.y = gridPos.y - (otherGridPos.x - gridPos.x);
                otherGridPos.x = xTmp;
            }
            break;
        default: throw std::runtime_error("Orientation de rotation invalide !");
    }
}
void RotationOperator::mapOperatorTypeInplace(OperatorType& otherType) const {
    switch (otherType) {
        case FLP_HOR: otherType = FLP_VRT; break;
        case FLP_VRT: otherType = FLP_HOR; break;
        case MOV_E: otherType = (type == ROT_CW) ? MOV_S : MOV_N; break;
        case MOV_S: otherType = (type == ROT_CW) ? MOV_W : MOV_E; break;
        case MOV_W: otherType = (type == ROT_CW) ? MOV_N : MOV_S; break;
        case MOV_N: otherType = (type == ROT_CW) ? MOV_E : MOV_W; break;
        default: break;
    }
}

void FlipOperator::mapGridPosInplace(sf::Vector2i& otherGridPos) const {
    switch (type) {
        case FLP_HOR:
            {   // Réflexion par rapport à l'axe des y : (x, y) -> (-x, y)
                otherGridPos.x = 2*gridPos.x - otherGridPos.x;
            }
            break;
        case FLP_VRT:
            {   // Réflexion par rapport à l'axe des x : (x, y) -> (x, -y)
                otherGridPos.y = 2*gridPos.y - otherGridPos.y;
            }
            break;
        default: throw std::runtime_error("Orientation de symétrie invalide !");
    }
}
void FlipOperator::mapOperatorTypeInplace(OperatorType& otherType) const {
    switch (otherType) {
        case ROT_CW: otherType = ROT_CCW; break;
        case ROT_CCW: otherType = ROT_CW; break;
        case MOV_E: if (type == FLP_HOR) otherType = MOV_W; break;
        case MOV_S: if (type == FLP_VRT) otherType = MOV_N; break;
        case MOV_W: if (type == FLP_HOR) otherType = MOV_E; break;
        case MOV_N: if (type == FLP_VRT) otherType = MOV_S; break;
        default: break;
    }
}