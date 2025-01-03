#include "Piece.hpp"
#include "Level.hpp"
#define _USE_MATH_DEFINES //pour M_PI
#include <cmath>

PieceColor::PieceColor(const sf::Color &primaryColor, const sf::Color &backgroundColor)
    : primaryColor{primaryColor}, backgroundColor{backgroundColor}
{}
const PieceColor PieceColor::WALL  {sf::Color{0x3F3F3FFF}, sf::Color{0xFFFFFCFF}};
const PieceColor PieceColor::RED   {sf::Color{0xFD4030FF}, sf::Color{0xFEB0A9FF}};
const PieceColor PieceColor::ORANGE{sf::Color{0xFFAA00FF}, sf::Color{0xFFE1A8FF}};
const PieceColor PieceColor::GREEN {sf::Color{0x7AE300FF}, sf::Color{0xDBFFB3FF}};
const PieceColor PieceColor::CYAN  {sf::Color{0x20C3DFFF}, sf::Color{0xB9F4F4FF}};
const PieceColor PieceColor::BLUE  {sf::Color{0x2779FCFF}, sf::Color{0xB4D0FEFF}};
const PieceColor PieceColor::PURPLE{sf::Color{0xBC39D6FF}, sf::Color{0xE5B3EFFF}};

PieceData::PieceData(const std::vector<sf::Vector2i> &gridCoords, const PieceColor &color)
    : gridCoords{gridCoords}
    , color{color}
{}

// PieceData::~PieceData()
// {
//     for (sf::Vector2f* vPtr : gridPosMoveOps)
//         delete vPtr;
// }

// PieceData::PieceData(const PieceData& other)
//     : gridCoords{other.gridCoords}
//     , color{other.color}
//     , gridPosMoveOps{
//         [&]{
//             std::vector<sf::Vector2f*> tmp{};
//             tmp.reserve(other.gridPosMoveOps.size());
//             for (sf::Vector2f* vPtr : other.gridPosMoveOps)
//                 tmp.emplace_back(vPtr ? new sf::Vector2f{*vPtr} : nullptr);
//             return tmp;
//         }()
//     }
// {
// }

// PieceData& PieceData::operator=(const PieceData& other)
// {
//     if (this == &other)
//         return *this;
//     PieceData temp{other};
//     gridCoords = std::move(temp.gridCoords);
//     color = std::move(temp.color);
//     for (size_t i = 0; i < gridPosMoveOps.size(); ++i)
//         std::swap(gridPosMoveOps[i], temp.gridPosMoveOps[i]);
//     return *this;
// }

void PieceData::emplaceCell(const sf::Vector2i& gridPos)
{
    gridCoords.emplace_back(gridPos);
}

Piece::Piece(const PieceData& data, Level& level, int pieceIdx)
    : PieceData{data}
    , DrawableShape{{}, sf::Triangles}
    , level{level}
    , pieceIdx{pieceIdx}
    , movementOperators{}
	, mainOperators{}
    , vertexArrayOld{}
    , animationTimer{nullptr}
    , hasCorrectPosition{false}
    , atCorrectPosition{false}
{}

Piece::~Piece()
{
    delete animationTimer;
}

const Level& Piece::getLevel() const { return level; }
const int Piece::getIndex() const { return pieceIdx; }
const bool Piece::isAtCorrectPosition() const { return !hasCorrectPosition || atCorrectPosition; }
const bool Piece::isInAnimation() const { return animationTimer; }
const bool Piece::canMoveInDirection(Direction direction) const { return movementOperators[direction]; }

void Piece::setHasCorrectPosition() { hasCorrectPosition = true; }
bool Piece::addOperator(Operator& op)
{
    switch (op.getType())
    {
    case MOV_E:
        if (movementOperators[E]) return false;
        movementOperators[E] = &op;
        return true;
    case MOV_S:
        if (movementOperators[S]) return false;
        movementOperators[S] = &op;
        return true;
    case MOV_W:
        if (movementOperators[W]) return false;
        movementOperators[W] = &op;
        return true;
    case MOV_N:
        if (movementOperators[N]) return false;
        movementOperators[N] = &op;
        return true;
    default:
        mainOperators.emplace_back(&op);
        return true;
    }
}

void Piece::transformWorldCoords(const sf::Transform& transform)
{
    if (!vertexArrayOld.size())
        vertexArrayOld = vertexArray;
    std::vector<sf::Vertex>::iterator it0{vertexArrayOld.begin()};
    std::vector<sf::Vertex>::iterator it1{vertexArray.begin()};
    for (; (it0 != vertexArrayOld.end()) && (it1 != vertexArray.end()); (++it0, ++it1))
        it1->position = transform.transformPoint(it0->position.x, it0->position.y);
}

void Piece::move(int direction, float progress)
{
    float offsetInPixels = progress * level.getGridSizeInPixels();

    switch (direction)
    {
    case 2 : // ouest
        offsetInPixels = -offsetInPixels;
    case 0 : // est
        transformWorldCoords({ 0.0f, 0.0f, offsetInPixels,
                               0.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 1.0f });
        break;
    case 3 : // nord
        offsetInPixels = -offsetInPixels;
    case 1 : // sud
        transformWorldCoords({ 0.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, offsetInPixels,
                               0.0f, 0.0f, 1.0f });
        break;
    }
}

void Piece::rotate(const sf::Vector2f& originWorldPos, float angleInRad)
{
    float cosine = std::cos(angleInRad);
    float sine = std::sin(angleInRad);
    float x = originWorldPos.x;
    float y = originWorldPos.y;
    transformWorldCoords({ cosine, -sine, - x*cosine + y*sine + x,
                           sine,  cosine, - x*sine - y*cosine + y,
                           0.0f, 0.0f, 1.0f });
}

void Piece::flipVertical(const sf::Vector2f& originWorldPos, float t)
{
    transformWorldCoords({ 1.0f-t, 0.0f, t * originWorldPos.x,
                           0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 1.0f });
}
void Piece::flipHorizontal(const sf::Vector2f& originWorldPos, float t)
{
    transformWorldCoords({ 1.0f, 0.0f, 0.0f,
                           0.0f, 1.0f-t, t * originWorldPos.y,
                           0.0f, 0.0f, 1.0f });
}

ActionResponse Piece::onClick(const sf::Vector2i& gridPos)
{
    std::cout << "Clicked Grid (" << gridPos.x << ", " << gridPos.y << ") of Piece " << pieceIdx << "; ";
    for (Operator* op : mainOperators) {
        if (op->getGridPosition() == gridPos) {
            return trigger(*op);
        }
    }
    std::cout << "Operation Invalid" << std::endl;
    return INVALID;
}
ActionResponse Piece::onSlide(Direction direction)
{
    std::cout << "Slided Piece " << pieceIdx << " in Direction " << direction << "; ";
    if (movementOperators[direction])
        return trigger(*movementOperators[direction]);
    std::cout << "Operation Invalid" << std::endl;
    return INVALID;
}

std::ostream& operator<<(std::ostream& stream, const sf::Vector2i& v) {
    return stream << '(' << v.x << ',' <<' ' << v.y << ')';
}
std::ostream& operator<<(std::ostream& stream, const sf::Vector2f& v) {
    return stream << '(' << v.x << ',' <<' ' << v.y << ')';
}


ActionResponse Piece::trigger(Operator& op)
{
    std::cout << "Triggered Operator of Type " << op.getType() << "; ";
    bool rejected = false;
    for (sf::Vector2i gridPos : gridCoords) {
        // oui, ici les `gridPos` sont bien des copies
        op.mapGridPosInplace(gridPos);
        int cellData = level.getCurrent(gridPos);
        if (cellData != 0 && cellData != pieceIdx + 2) {
            rejected = true;
            break;
        }
    }
    if (rejected) {
        reject(op);
        std::cout << "Operator Rejected" << std::endl;
        return REJECTED;
    } else {
        accept(op);
        std::cout << "Operator Accepted" << std::endl;
        return ACCEPTED;
    }
}

void Piece::accept(Operator& op)
{
    for (sf::Vector2i& gridPos : gridCoords) {
        // ici les `gridPos` sont des références
        level.setCurrent(gridPos, 0);
        op.mapGridPosInplace(gridPos);
    }
    for (sf::Vector2i& gridPos : gridCoords) {
        // ici les `gridPos` sont des références
        level.setCurrent(gridPos, pieceIdx + 2);
    }
    for (Operator* otherOp : movementOperators) {
        if (otherOp) otherOp->acceptOperator(op);
    }
    for (Operator* otherOp : mainOperators) {
        otherOp->acceptOperator(op);
    }
    switch (op.getType())
    {
    case ROT_CW:
        std::swap(movementOperators[N], movementOperators[W]);
        std::swap(movementOperators[W], movementOperators[S]);
        std::swap(movementOperators[S], movementOperators[E]);
        break;
    case ROT_CCW:
        std::swap(movementOperators[N], movementOperators[E]);
        std::swap(movementOperators[E], movementOperators[S]);
        std::swap(movementOperators[S], movementOperators[W]);
        break;
    case FLP_HOR:
        std::swap(movementOperators[E], movementOperators[W]);
        break;
    case FLP_VRT:
        std::swap(movementOperators[S], movementOperators[N]);
        break;
    default: break;
    }
    
    update();
}

void Piece::reject(Operator& op)
{
}

// void Piece::trigger(const sf::Vector2i& gridPos)
// { // TODO
//     std::cout << "Piece " << pieceIdx
//               << " trigger " << gridPos.x << " " << gridPos.y << std::endl;
//     if (pieceIdx % 2)
//         flipHorizontal(level.getCenterCoords(), .3f);
//     else
//         rotate(level.getCenterCoords(), .2f);
// }

void Piece::update()
{
    vertexArray.clear();
    vertexArray.reserve(gridCoords.size() * 6);
    for (const sf::Vector2i& gridPos : gridCoords)
        level.addSquareTo(vertexArray, gridPos, color.primaryColor);

    for (Operator* op : movementOperators)
        if (op) op->update();
    for (Operator* op : mainOperators)
        op->update();
}
void Piece::initializeAnimation()
{
    vertexArrayOld = vertexArray;
    animationTimer = new sf::Clock{};
}
void Piece::endAnimation()
{
    vertexArrayOld.clear();
    delete animationTimer;
}

void Piece::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    DrawableShape::draw(target, states);
    for (const Operator* op : movementOperators)
        if (op) target.draw(*op, states);
    for (const Operator* op : mainOperators)
        target.draw(*op, states);
}