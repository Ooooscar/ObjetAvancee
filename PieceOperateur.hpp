#ifndef _PIECEOUT_PIECE_OPERATEUR
#define _PIECEOUT_PIECE_OPERATEUR
#include <utility> // pour pair
using std::pair;

class OperateurDeplacement;
class OperateurRotation;
class OperateurSymetrie;

class PieceOperateur
{
protected:
	pair<int, int> position;
public:
	PieceOperateur(const pair<int,int> &position);
	virtual void mapPosition(pair<int, int> &pos) const = 0;
	virtual void mapOperateur(PieceOperateur &op) const = 0;

	virtual void accept(const OperateurDeplacement &op);
	virtual void accept(const OperateurRotation &op);
	virtual void accept(const OperateurSymetrie &op);
	// virtual void visit(Piece &p) const;
	
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
	OperateurDeplacement(const pair<int,int> &position, Orientation sens);
	virtual void mapPosition(pair<int, int> &pos) const;
	virtual void mapOperateur(PieceOperateur &op) const;

	virtual void accept(const OperateurRotation &op);
	virtual void accept(const OperateurSymetrie &op);
};

class OperateurRotation : public PieceOperateur
{
public:
	enum Orientation {HORAIRE, ANTIHORAIRE};
public:
	Orientation sens;
public:
	OperateurRotation(const pair<int,int> &position, Orientation sens);
	virtual void mapPosition(pair<int, int> &pos) const;
	virtual void mapOperateur(PieceOperateur &op) const;

	virtual void accept(const OperateurSymetrie &op);
};

class OperateurSymetrie : public PieceOperateur
{
public:
	enum Orientation {VERTICALE, HORIZONTALE};
	Orientation sens;
public:
	OperateurSymetrie(const pair<int,int> &position, Orientation sens);
	virtual void mapPosition(pair<int, int> &pos) const;
	virtual void mapOperateur(PieceOperateur &op) const;

	virtual void accept(const OperateurRotation &op);
};

#endif