#ifndef _PIECEOUT_EBAUCHE_MODEL
#define _PIECEOUT_EBAUCHE_MODEL
#include <utility> // pour pair
#include <vector>
#include <SFML/Graphics.hpp>
using std::vector;
using std::pair;

class Piece;
class Piece;
class PieceOperateur;
class OperateurDeplacement;
class CouleurPiece;
enum OrientationDeplacement {NORD, SUD, EST, OUEST};
enum OrientationSymetrie {HORIZONTALE, VERTICALE};
enum OrientationRotation {HORAIRE, ANTIHORAIRE};

class Piece
{
public:
	vector<pair<int, int>> coordinates;
	const vector<PieceOperateur*> operateurs;
	const CouleurPiece &couleur;
public:
	Piece(const vector<pair<int, int>> &coords, const vector<PieceOperateur*> &operateurs, const CouleurPiece &couleur);
	const vector<pair<int, int>>& getCoordinates() const;
	void trigger(const pair<float, float> &pos);
	// void accept(const PieceOperateur &v);
	void accept(PieceOperateur &v);
};

class PieceOperateur
{
protected:
	Piece &source;
	pair<int, int> position;
public:
	PieceOperateur(Piece &source, const pair<int,int> &position);
	// PieceOperateur(const pair<int,int> &position);
	// const vector<pair<int, int>>& getCoordinates() const;
	virtual bool trigger(const pair<float, float> &pos);
	
	virtual void mapPosition(pair<int, int> &pos) const = 0; 
	// virtual void visit(Piece &p) const;
	
	// virtual void visit(OperateurDeplacement &) const = 0;
	//virtual void visit(OperateurRotation &) const = 0;
	//virtual void visit(OperateurSymetrie &) const = 0;
	
	virtual void accept(const PieceOperateur &v) = 0;
};

class OperateurDeplacement : public PieceOperateur
{
public:
	const OrientationDeplacement sens;
	OperateurDeplacement(Piece &source, const pair<int,int> &position, OrientationDeplacement sens);
	// OperateurDeplacement(const pair<int,int> &position, OrientationDeplacement sens);
	virtual void accept(const PieceOperateur &v);
	// virtual void visit(OperateurDeplacement &x) const;
	//virtual void visit(OperateurRotation &x) const;
	//virtual void visit(OperateurSymetrie &x) const;
	
	virtual void mapPosition(pair<int, int> &pos) const;
};

class CouleurPiece : public std::pair<const sf::Color, const sf::Color>
{
public:
    CouleurPiece(const sf::Color &piece, const sf::Color &cible);
    static const CouleurPiece ROUGE;
    static const CouleurPiece ORANGE;
    static const CouleurPiece VERT;
    static const CouleurPiece CYAN;
    static const CouleurPiece BLEU;
    static const CouleurPiece VIOLET;
};

#endif