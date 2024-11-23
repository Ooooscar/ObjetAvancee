#ifndef _PIECEOUT_EBAUCHE_MODEL
#define _PIECEOUT_EBAUCHE_MODEL
#include <utility> // pour pair
#include <vector>
#include <SFML/Graphics.hpp>
using std::vector;
using std::pair;

class Piece;
class PieceConcrete;
class PieceOperateur;
class OperateurDeplacement;
enum OrientationDeplacement {NORD, SUD, EST, OUEST};

class Piece {
public:
	virtual void trigger(const pair<int,int> &coord, Piece &origin) = 0;
	void trigger(const pair<int,int> &coord) { trigger(coord, *this); }
	virtual const vector<pair<int, int>>& getCoordinates() const = 0;
	virtual void accept(const PieceOperateur &v) = 0;
};

class PieceConcrete : public Piece {
public:
	vector<pair<int, int>> coordinates;
	const sf::Color& couleur;
public:
	PieceConcrete(const vector<pair<int, int>> &coords, const sf::Color &couleur);
	const vector<pair<int, int>>& getCoordinates() const;
	void trigger(const pair<int,int> &coord, Piece &origin) {}
	void accept(const PieceOperateur &v);
};

class PieceOperateur : public Piece {
protected:
	Piece& source;
	pair<int, int> position;
public:
	PieceOperateur(Piece &source, const pair<int,int> &position);
	const vector<pair<int, int>>& getCoordinates() const;
	void trigger(const pair<int,int> &relativePos, Piece &origin);
	
	virtual void mapPosition(pair<int, int> &pos) const = 0; 
	virtual void visit(PieceConcrete &p) const;
	
	virtual void visit(OperateurDeplacement &) const = 0;
	//virtual void visit(OperateurRotation &) const = 0;
	//virtual void visit(OperateurSymetrie &) const = 0;
	
	virtual void accept(const PieceOperateur &v) = 0;
};

class OperateurDeplacement : public PieceOperateur
{
public:
	const OrientationDeplacement sens;
	OperateurDeplacement(Piece &source, const pair<int,int> &position, OrientationDeplacement sens);
	virtual void accept(const PieceOperateur &v);
	virtual void visit(OperateurDeplacement &x) const;
	//virtual void visit(OperateurRotation &x) const;
	//virtual void visit(OperateurSymetrie &x) const;
	
	virtual void mapPosition(pair<int, int> &pos) const;
};

// int main()
// {
// 	vector<pair<int, int>> coords {{0, 0}, {0, 1}, {0, 2}, {1, 2}};     
// 	PieceConcrete tetris_L(coords);
// 	Piece *p = new OperateurDeplacement {tetris_L, {0, 0}, EST};
// 	p->trigger(std::make_pair(0, 0));
// 	p->trigger(std::make_pair(1, 0));
// }

#endif