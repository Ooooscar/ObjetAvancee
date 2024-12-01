#ifndef _PIECEOUT_LEVEL
#define _PIECEOUT_LEVEL
#include <utility>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Piece.hpp"

class NiveauData
{
protected:
    int nbCol;
    int nbLigne;
    std::vector<int> casesAttendues;
    std::vector<Piece> dataPieces;
public:
    NiveauData(const int nbCol, const int nbLigne, const std::vector<int> &casesAttendues);
    NiveauData(const int nbCol, const int nbLigne, const std::vector<int> &&casesAttendues);
    Piece& ajoutePiece(const std::vector<pair<int, int>> &coords, const CouleurPiece &couleur);
};

class Niveau : public NiveauData
{
public:
    std::vector<Piece> pieces;
private:
    std::vector<int> casesActuelles;
    int nbCasesOccupees;
public:
    Niveau(const NiveauData &niveauData);
    const int getNbCol() const;
    const int getNbLigne() const;
    const int getNbPieces() const;
    const int getNbCasesOccupees() const;

    const int getDataActuelleParIndice(int indice) const;
    const int getDataActuelle(int x, int y) const;
    const int getDataAttendueParIndice(int indice) const;
    const int getDataAttendue(int x, int y) const;
    const sf::Color& getCouleurPiece(int indicePiece) const;
    const sf::Color& getCouleurPieceSecondaire(int indicePiece) const;

    void setData(int x, int y, int value);
    void triggerPiece(int indice, int mouseX, int mouseY);
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
    int nbCol;
    int nbLigne;

    sf::RectangleShape *const panneauCentral = new sf::RectangleShape;
    std::vector<sf::Vector2f> treillis;
    std::vector<sf::Vertex> sommetsTrame;
    std::vector<sf::Vertex> sommetsSceneParticuliere;

public:
    AfficheurNiveau(sf::RenderWindow& fenetre, const std::vector<NiveauData>& niveaux);
    void prochainNiveau();
    void allerAuNiveau(int indice);
    void dessiner();
    void demarrer();
private:
    void initialiseNiveau();
    void initialiseTrame();
    void genereTreillis();
    void ajouteSommetsCellule(std::vector<sf::Vertex>& trame, int& x, int& y, const sf::Color& couleur);
};

#endif