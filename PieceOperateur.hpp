#ifndef _PIECEOUT_PIECE_OPERATEUR
#define _PIECEOUT_PIECE_OPERATEUR
#include <memory> // pour unique_ptr
#include <utility> // pour pair

class Piece;
class OperateurDeplacement;
class OperateurRotation;
class OperateurSymetrie;

class PieceOperateur
{
protected:
	std::pair<int, int> position;
	virtual PieceOperateur* clone() const = 0;
	friend class PieceData;

public:
	PieceOperateur(const std::pair<int,int> &position);
	virtual ~PieceOperateur() = default;

	const std::pair<int,int> &getPosition() const;

	virtual void mapPosition(std::pair<int, int> &pos) const = 0;
	virtual void mapOperateur(PieceOperateur &op) const = 0;

	virtual void accepter(const OperateurDeplacement &op);
	virtual void accepter(const OperateurRotation &op);
	virtual void accepter(const OperateurSymetrie &op);
	// virtual void visit(PieceData &p) const;
	
	// virtual void visit(OperateurDeplacement &) const = 0;
	//virtual void visit(OperateurRotation &) const = 0;
	//virtual void visit(OperateurSymetrie &) const = 0;
};

class OperateurDeplacement : public PieceOperateur
{
public:
	enum Orientation {EST, SUD, OUEST, NORD};
private:
	Orientation sens;
public:
	OperateurDeplacement(const std::pair<int,int> &position, Orientation sens);
	virtual PieceOperateur* clone() const;

	virtual void mapPosition(std::pair<int, int> &pos) const;
	virtual void mapOperateur(PieceOperateur &op) const;

	virtual void accepter(const OperateurRotation &op);
	virtual void accepter(const OperateurSymetrie &op);
};

class OperateurRotation : public PieceOperateur
{
public:
	enum Orientation {HORAIRE, ANTIHORAIRE};
public:
	Orientation sens;
public:
	OperateurRotation(const std::pair<int,int> &position, Orientation sens);
	virtual PieceOperateur* clone() const;

	virtual void mapPosition(std::pair<int, int> &pos) const;
	virtual void mapOperateur(PieceOperateur &op) const;

	virtual void accepter(const OperateurSymetrie &op);
};

class OperateurSymetrie : public PieceOperateur
{
public:
	enum Orientation {VERTICALE, HORIZONTALE};
	Orientation sens;
public:
	OperateurSymetrie(const std::pair<int,int> &position, Orientation sens);
	virtual PieceOperateur* clone() const;

	virtual void mapPosition(std::pair<int, int> &pos) const;
	virtual void mapOperateur(PieceOperateur &op) const;

	virtual void accepter(const OperateurRotation &op);
};

#endif