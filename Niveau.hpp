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
    std::vector<PieceData> dataPieces;
public:
    NiveauData(const int nbCol, const int nbLigne, const std::vector<int> &casesAttendue);
    NiveauData(const int nbCol, const int nbLigne, std::vector<int> &&casesAttendue);
    PieceData& ajouterPiece(const std::vector<std::pair<int, int>> &coords, const CouleurPiece &couleur);
};

class Niveau : private NiveauData, public sf::Drawable
{
public:
    static const int MARGIN_LEFT = 100, MARGIN_RIGHT = 100, MARGIN_TOP = 100, MARGIN_BOTTOM = 100;
    static const int TILE_SIZE = 96;

private:
    static const sf::Color COULEUR_DU_MUR;
    static const sf::Color COULEUR_DU_SOL;

    std::vector<Piece> pieces;
    std::vector<int> dataCasesActuelle;
    int nbCasesOccupees;

    sf::RectangleShape panneauCentral;
    std::vector<sf::Vector2f> treillis;
    std::vector<sf::Vertex> sommetsTrame;

    friend class Piece;

public:
    Niveau(const NiveauData &dataNiveau);
    const int getDataActuelle(const std::pair<int, int> &caseChoisie) const;
    const int getDataAttendue(const std::pair<int, int> &caseChoisie) const;
    void redefinirData(const std::pair<int, int> &caseChoisie, int valeur);
    bool triggerPiece(int indicePiece, const std::pair<int, int> &caseChoisie);
    const bool estGagne() const;
    
    const bool contient(const sf::Vector2f& posSouris) const;
    std::pair<int, int> mapPixelsEnCases(const sf::Vector2f& posSouris) const;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
    void genererTreillis();
    void ajouterSommetsCellule(std::vector<sf::Vertex>& trame, int x, int y, const sf::Color& couleur);
};

class AfficheurNiveau
{
private:
    static const int MARGIN_LEFT = 100, MARGIN_RIGHT = 100, MARGIN_TOP = 100, MARGIN_BOTTOM = 100;
	static const int TILE_SIZE = 96;
    static const sf::Color COULEUR_DU_MUR;
    static const sf::Color COULEUR_DU_SOL;

    sf::RenderWindow& fenetre;
    const std::vector<NiveauData> niveaux;
    int indiceNiveauActuel;
    Niveau niveauActuel;

public:
    AfficheurNiveau(sf::RenderWindow& fenetre, const std::vector<NiveauData>& niveaux);
    void prochainNiveau();
    void allerAuNiveau(int indice);
    void demarrer();
};

#endif