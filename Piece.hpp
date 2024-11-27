#ifndef _PIECEOUT_EBAUCHE_MODEL
#define _PIECEOUT_EBAUCHE_MODEL
#include <utility> // pour pair
#include <vector>
#include <SFML/Graphics.hpp>
#include "PieceOperateur.hpp"

class CouleurPiece;

class Piece
{
public:
	std::vector<pair<int, int>> coordinates;
	std::vector<PieceOperateur*> operateurs;
	const CouleurPiece &couleur;
public:
	Piece(const std::vector<pair<int, int>> &coords, const std::vector<PieceOperateur*> &operateurs, const CouleurPiece &couleur);
	const std::vector<pair<int, int>>& getCoordinates() const;
	OperateurDeplacement& ajouteOpDeplacement(const pair<int, int> &position, OperateurDeplacement::Orientation sens);
	OperateurRotation& ajouteOpRotation(const pair<int, int> &position, OperateurRotation::Orientation sens);
	OperateurSymetrie& ajouteOpSymetrie(const pair<int, int> &position, OperateurSymetrie::Orientation sens);
	void trigger(const pair<float, float> &pos);
	void accept(PieceOperateur &op);
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