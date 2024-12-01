#ifndef _PIECEOUT_EBAUCHE_MODEL
#define _PIECEOUT_EBAUCHE_MODEL
#include <utility> // pour pair
#include <vector>
#include <SFML/Graphics.hpp>
#include "PieceOperateur.hpp"

class CouleurPiece : public std::pair<sf::Color, sf::Color>
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

class Piece
{
public:
	std::vector<pair<int, int>> coordinates;
	std::vector<PieceOperateur*> operateurs;
	CouleurPiece couleur;
private:
	std::vector<sf::Vertex> sommets;
public:
	Piece(const std::vector<pair<int, int>> &coords, const CouleurPiece &couleur);
	const std::vector<pair<int, int>>& getCoordinates() const;
	OperateurDeplacement& ajouteOpDeplacement(const pair<int, int> &position, OperateurDeplacement::Orientation sens);
	OperateurRotation& ajouteOpRotation(const pair<int, int> &position, OperateurRotation::Orientation sens);
	OperateurSymetrie& ajouteOpSymetrie(const pair<int, int> &position, OperateurSymetrie::Orientation sens);
	void trigger(int mouseX, int mouseY, std::vector<int> &dataActuel);
	void accept(PieceOperateur &op, std::vector<int> &dataActuel);
	// virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif