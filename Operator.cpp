#include "Operator.hpp"
#include "Piece.hpp"
#include "Level.hpp"
#define _USE_MATH_DEFINES //pour M_PI
#include <array>
#include <cmath>

const std::array<const std::vector<sf::Vector2f>, OperatorType::Count> vertexPatterns
{
    // movement east
    std::vector<sf::Vector2f>{{0.88f, 0.5f}, {0.7f, 0.36f}, {0.7f, 0.64f}},
    // movement south
    std::vector<sf::Vector2f>{{0.5f, 0.88f}, {0.36f, 0.7f}, {0.64f, 0.7f}},
    // movement west
    std::vector<sf::Vector2f>{{0.12f, 0.5f}, {0.3f, 0.36f}, {0.3f, 0.64f}},
    // movement north
    std::vector<sf::Vector2f>{{0.5f, 0.12f}, {0.36f, 0.3f}, {0.64f, 0.3f}},
    // rotation clockwise
    std::vector<sf::Vector2f>{{0.2f, 0.2f}, {0.2f, 0.8f}, {0.8f, 0.2f}, {0.8f, 0.8f}},
    // rotation counterclockwise
    std::vector<sf::Vector2f>{{0.2f, 0.2f}, {0.2f, 0.8f}, {0.8f, 0.2f}, {0.8f, 0.8f}},
    // flip horizontal
    std::vector<sf::Vector2f>{{0.45f, 0.2f}, {0.55f, 0.2f}, {0.45f, 0.8f}, {0.55f, 0.8f}},
    // flip vertical
    std::vector<sf::Vector2f>{{0.2f, 0.45f}, {0.2f, 0.55f}, {0.8f, 0.45f}, {0.8f, 0.55f}},
};

OperatorData::OperatorData(OperatorType type, const sf::Vector2i& gridPos)
    : type{type}
    , gridPos{gridPos}
{}

Operator::Operator(Level& level, Piece& owner, OperatorData& data)
    : Operator(level, owner, data, level.mapGridToPixel(data.gridPos))
{}

Operator::Operator(Level& level, Piece& owner, OperatorData& data, sf::Vector2f worldPos)
    : DrawableShape{std::vector<sf::Vertex>(vertexPatterns[data.type].size()), sf::TriangleStrip}
    , level{level}
    , owner{owner}
    , data{data}
    , worldPos{worldPos}
{}

OperatorType Operator::getType() const { return data.type; }
const sf::Vector2i& Operator::getGridPosition() const { return data.gridPos; }

void Operator::setGridPosition(const sf::Vector2i& otherGridPos)
{
    data.gridPos = otherGridPos;
    worldPos = owner.getLevel().mapGridToPixel(data.gridPos);
}

void Operator::mapGridPosInplace(sf::Vector2i& otherGridPos) const
{
    switch (data.type)
    {
    case MOV_E: ++otherGridPos.x; break;
    case MOV_S: ++otherGridPos.y; break;
    case MOV_W: --otherGridPos.x; break;
    case MOV_N: --otherGridPos.y; break;
    case ROT_CW:
        {   // Rotation 90° horarire : (x, y) -> (y, -x)
            int xTmp = data.gridPos.x - (otherGridPos.y - data.gridPos.y);
            otherGridPos.y = data.gridPos.y + (otherGridPos.x - data.gridPos.x);
            otherGridPos.x = xTmp;
        }
        break;
    case ROT_CCW:
        {   // Rotation 90° anti-horarire : (x, y) -> (-y, x)
            int xTmp = data.gridPos.x + (otherGridPos.y - data.gridPos.y);
            otherGridPos.y = data.gridPos.y - (otherGridPos.x - data.gridPos.x);
            otherGridPos.x = xTmp;
        }
        break;
    case FLP_HOR:
        {   // Réflexion par rapport à l'axe des y : (x, y) -> (-x, y)
            otherGridPos.x = 2*data.gridPos.x - otherGridPos.x;
        }
        break;
    case FLP_VRT:
        {   // Réflexion par rapport à l'axe des x : (x, y) -> (x, -y)
            otherGridPos.y = 2*data.gridPos.y - otherGridPos.y;
        }
        break;
    default:
        break;
    }
}
bool Operator::acceptOperator(const Operator& other)
{
    other.mapGridPosInplace(data.gridPos);
    switch (other.data.type)
    {
    case ROT_CW:
        switch (data.type)
        {
        case MOV_E: data.type = MOV_S; break;
        case MOV_S: data.type = MOV_W; break;
        case MOV_W: data.type = MOV_N; break;
        case MOV_N: data.type = MOV_E; break;
        case FLP_HOR: data.type = FLP_VRT; break;
        case FLP_VRT: data.type = FLP_HOR; break;
        default: return false;
        }
        break;
    case ROT_CCW:
        switch (data.type)
        {
        case MOV_E: data.type = MOV_N; break;
        case MOV_S: data.type = MOV_E; break;
        case MOV_W: data.type = MOV_S; break;
        case MOV_N: data.type = MOV_W; break;
        case FLP_HOR: data.type = FLP_VRT; break;
        case FLP_VRT: data.type = FLP_HOR; break;
        default: return false;
        }
        break;
    case FLP_HOR:
        switch (data.type)
        {
        case MOV_E: data.type = MOV_W; break;
        case MOV_W: data.type = MOV_E; break;
        case ROT_CW: data.type = ROT_CCW; break;
        case ROT_CCW: data.type = ROT_CW; break;
        default: return false;
        }
        break;
    case FLP_VRT:
        switch (data.type)
        {
        case MOV_S: data.type = MOV_N; break;
        case MOV_N: data.type = MOV_S; break;
        case ROT_CW: data.type = ROT_CCW; break;
        case ROT_CCW: data.type = ROT_CW; break;
        default: return false;
        }
        break;
    default: return false;
    }
    return true;
}

void Operator::accept()
{
}
void Operator::reject()
{
}

void Operator::update()
{
    float gridSizeInPixels = level.getGridSizeInPixels();
    vertexArray.clear();
    for (const sf::Vector2f& v : vertexPatterns[data.type])
        vertexArray.emplace_back(level.mapGridToPixel(data.gridPos) + gridSizeInPixels * v);
}