#ifndef _PIECEOUT_EBAUCHE_MODEL
#define _PIECEOUT_EBAUCHE_MODEL
#include "PieceOperateur.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
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
// protected:
public:
	std::vector<std::pair<int, int>> coordonnees;
	std::vector<PieceOperateurData*> operateurs;
	CouleurPiece couleur;
public:
	virtual ~PieceData();
	PieceData(const std::vector<std::pair<int, int>> &coords, const CouleurPiece &couleur);
	PieceData(const PieceData& other);
	const std::vector<std::pair<int, int>>& getCoordonnees() const;
    const sf::Color& getCouleur() const;
    const sf::Color& getCouleurSecondaire() const;
private:
	void ajouterOpDeplacement(const std::pair<int, int> &position, OperateurDeplacement::Orientation sens);
	void ajouterOpRotation(const std::pair<int, int> &position, OperateurRotation::Orientation sens);
	void ajouterOpSymetrie(const std::pair<int, int> &position, OperateurSymetrie::Orientation sens);
};

class Niveau;

class Piece : public PieceData, public sf::Drawable
{
private:
	Niveau &niveau;
	const int indicePiece;
	std::vector<sf::Vertex> sommets;
	bool auBonEndroit;
	bool enMouvement;
	friend class Niveau;
public:
	Piece(Niveau &niveau, int indicePiece, const PieceData &dataPiece);
	const int getIndice() const;
	const bool estAuBonEndroit() const;

	bool trigger(const std::pair<int, int> &caseChoisie);
	bool accepter(PieceOperateurData &op);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif