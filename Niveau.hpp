#ifndef _PIECEOUT_LEVEL
#define _PIECEOUT_LEVEL
#include "Piece.hpp"
#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>

class NiveauData
{
protected:
    int nbCol;
    int nbLigne;
    std::vector<int> dataCasesAttendue;
    std::vector<int> dataCasesActuelle;
	std::vector<PieceOperateurData*> dataOperateurs;
    std::vector<CouleurPiece> couleurs;
public:
    NiveauData( const int nbCol, const int nbLigne,
                std::vector<int> &&casesAttendues, std::vector<int> &&casesActuelles,
                std::initializer_list<PieceOperateurData*> operateurs,
                std::initializer_list<CouleurPiece> couleurs );
    const int getDataAttendue(const std::pair<int, int> &caseChoisie) const;
    const int getDataActuelle(const std::pair<int, int> &caseChoisie) const;
    void redefinirData(const std::pair<int, int> &caseChoisie, int valeur);
};

class Niveau : public NiveauData, public sf::Drawable
{
private:
    static const sf::Color COULEUR_DU_MUR;
    static const sf::Color COULEUR_DU_SOL;

    std::vector<Piece> pieces;
    sf::Clock minuterieAnimation;
    Piece* pieceEnMouvement;
    bool gagne;

    float tailleCase;
    sf::RectangleShape panneauCentral;
    std::vector<sf::Vertex> sommetsTrame;

    friend class Piece;

public:
    Niveau(const NiveauData &dataNiveau, const sf::Vector2f& coordCentre, float tailleCase);
    bool triggerPiece(int indicePiece, const std::pair<int, int> &caseChoisie);
    const bool estEnMouvement() const;
    const bool estGagne() const;
    
    const bool contient(const sf::Vector2f &posSouris) const;
    std::pair<int, int> mapPixelsEnCases(const sf::Vector2f &posSouris) const;
    void updateGagne();
    void updateAnimation();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
    // void ajouterSommetsCellule(std::vector<sf::Vertex>& trame, int x, int y, const sf::Color& couleur);
    void ajouterSommetsCellule(std::vector<sf::Vertex>& trame, float x, float y, const sf::Color& couleur);
};

#endif