#ifndef _PIECEOUT_EBAUCHE_MODEL
#define _PIECEOUT_EBAUCHE_MODEL
#include "PieceOperateur.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <utility> // pour pair

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

class PieceData
{
protected:
	std::vector<std::pair<int, int>> coordonnees;
	std::vector<PieceOperateur*> operateurs;
	CouleurPiece couleur;
public:
	PieceData(const std::vector<std::pair<int, int>> &coords, const CouleurPiece &couleur);
	OperateurDeplacement& ajouterOpDeplacement(const std::pair<int, int> &position, OperateurDeplacement::Orientation sens);
	OperateurRotation& ajouterOpRotation(const std::pair<int, int> &position, OperateurRotation::Orientation sens);
	OperateurSymetrie& ajouterOpSymetrie(const std::pair<int, int> &position, OperateurSymetrie::Orientation sens);
};

class Niveau;

class Piece : private PieceData, public sf::Drawable
{
private:
	Niveau &niveau;
	const int indice;
	std::vector<sf::Vertex> sommets;
	friend class Niveau;
public:
	// Piece(int indice, const PieceData &dataPiece, int tailleCase);
	Piece(Niveau &niveau, int indice, const PieceData &dataPiece);
	const int getIndice() const;
	const std::vector<std::pair<int, int>>& getCoordonnees() const;
    const sf::Color& getCouleur() const;
    const sf::Color& getCouleurSecondaire() const;

	void trigger(const std::pair<int, int> &caseChoisie, std::vector<int> &casesActuelles);
	void accepter(PieceOperateur &op, std::vector<int> &casesActuelles);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif