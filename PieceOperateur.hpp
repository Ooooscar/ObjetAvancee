#ifndef _PIECEOUT_PIECE_OPERATEUR
#define _PIECEOUT_PIECE_OPERATEUR
#include <memory> // pour unique_ptr
#include <utility> // pour pair
#include <SFML/Graphics.hpp>

class Piece;
class OperateurDeplacement;
class OperateurRotation;
class OperateurSymetrie;

class PieceOperateurData
{
protected:
	std::pair<int, int> position;
	friend class PieceData;

public:
	PieceOperateurData(const std::pair<int,int> &position);
	virtual ~PieceOperateurData() = default;
	virtual PieceOperateurData* clone() const = 0;

	const std::pair<int,int> &getPosition() const;

	virtual void mapPosition(std::pair<int, int> &pos) const = 0;
	virtual void mapPosition(sf::Vector2f &pos, float t) const = 0;
	virtual void mapOperateur(PieceOperateurData &op) const = 0;

	virtual void accepter(const OperateurDeplacement &op);
	virtual void accepter(const OperateurRotation &op);
	virtual void accepter(const OperateurSymetrie &op);
};

class OperateurDeplacement : public PieceOperateurData
{
public:
	enum Orientation {EST, SUD, OUEST, NORD};
private:
	Orientation sens;
public:
	OperateurDeplacement(const std::pair<int,int> &position, Orientation sens);
	virtual PieceOperateurData* clone() const;
	const Orientation getSens() const;

	virtual void mapPosition(std::pair<int, int> &pos) const;
	virtual void mapPosition(sf::Vector2f &pos, float t) const;
	virtual void mapOperateur(PieceOperateurData &op) const;

	virtual void accepter(const OperateurRotation &op);
	virtual void accepter(const OperateurSymetrie &op);
};

class OperateurRotation : public PieceOperateurData
{
public:
	enum Orientation {HORAIRE, ANTIHORAIRE};
private:
	Orientation sens;
public:
	OperateurRotation(const std::pair<int,int> &position, Orientation sens);
	virtual PieceOperateurData* clone() const;
	const Orientation getSens() const;

	virtual void mapPosition(std::pair<int, int> &pos) const;
	virtual void mapPosition(sf::Vector2f &pos, float t) const;
	virtual void mapOperateur(PieceOperateurData &op) const;

	virtual void accepter(const OperateurSymetrie &op);
};

class OperateurSymetrie : public PieceOperateurData
{
public:
	enum Orientation {VERTICALE, HORIZONTALE};
private:
	Orientation sens;
public:
	OperateurSymetrie(const std::pair<int,int> &position, Orientation sens);
	virtual PieceOperateurData* clone() const;
	const Orientation getSens() const;

	virtual void mapPosition(std::pair<int, int> &pos) const;
	virtual void mapPosition(sf::Vector2f &pos, float t) const;
	virtual void mapOperateur(PieceOperateurData &op) const;

	virtual void accepter(const OperateurRotation &op);
};

#endif