#ifndef _PIECEOUT_OPERATOR
#define _PIECEOUT_OPERATOR
#include "DrawableShape.hpp"
#include <SFML/Graphics.hpp>

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

class Operator;

class OperatorFactory
{
private:
	const Level& level;
public:
	OperatorFactory(const Level& level);
	Operator* createOperator(const OperatorData& data, Piece& source) const;
};

class Operator : public OperatorData, public DrawableShape
{
protected:
	Piece& source;
	sf::Vector2f worldPos;

	Operator(const OperatorData& data, Piece& source, sf::Vector2f&& worldPos);
	friend class OperatorFactory;
public:
	OperatorType getType() const;
	const sf::Vector2i& getGridPosition() const;

	void setGridPosition(const sf::Vector2i& otherGridPos);

	virtual void mapGridPosInplace(sf::Vector2i& otherGridPos) const = 0;
	virtual void mapOperatorTypeInplace(OperatorType& type) const = 0;

	virtual void rebuildMesh();
};

class MovementOperator : public Operator
{
protected:
	using Operator::Operator;
public:
	void mapGridPosInplace(sf::Vector2i& otherGridPos) const override;
	void mapOperatorTypeInplace(OperatorType& type) const override;
};

class RotationOperator : public Operator
{
protected:
	using Operator::Operator;
public:
	void mapGridPosInplace(sf::Vector2i& otherGridPos) const override;
	void mapOperatorTypeInplace(OperatorType& type) const override;
};

class FlipOperator : public Operator
{
protected:
	using Operator::Operator;
public:
	void mapGridPosInplace(sf::Vector2i& otherGridPos) const override;
	void mapOperatorTypeInplace(OperatorType& otherType) const override;
};

#endif