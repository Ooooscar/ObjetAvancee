#ifndef _PIECEOUT_OPERATOR
#define _PIECEOUT_OPERATOR
#include <SFML/Graphics.hpp>
#include "DrawableShape.hpp"

enum OperatorType
{
	MOV_E,   // movement east
	MOV_S,   // movement south
	MOV_W,   // movement west
	MOV_N,   // movement north
	ROT_CW,  // rotation clockwise
	ROT_CCW, // rotation counterclockwise
	FLP_HOR, // flip horizontal
	FLP_VRT, // flip vertical
	Count
};

class Level;
class Piece;

class OperatorData
{
public:
	OperatorType type;
	sf::Vector2i gridPos;
	OperatorData(OperatorType type, const sf::Vector2i& gridPos);
};

class Operator : public DrawableShape
{
private:
	Level& level;
	Piece& owner;
	OperatorData& data;
	sf::Vector2f worldPos;

	Operator(Level& level, Piece& owner, OperatorData& data, sf::Vector2f worldPos);
public:
	Operator(Level& level, Piece& owner, OperatorData& data);

	OperatorType getType() const;
	const sf::Vector2i& getGridPosition() const;

	void setGridPosition(const sf::Vector2i& otherGridPos);

	void mapGridPosInplace(sf::Vector2i& otherGridPos) const;
	bool acceptOperator(const Operator& other);

	void accept();
	void reject();

	void update();
};

#endif