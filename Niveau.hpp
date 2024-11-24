#ifndef _PIECEOUT_LEVEL
#define _PIECEOUT_LEVEL
#include <iostream>
#include <SFML/Graphics.hpp>
#include "ebaucheModel.hpp"

class Niveau
{
public:
    const int nbCol;
    const int nbLigne;
    std::vector<int> levelData;
    const std::vector<PieceConcrete*> piecesConcretes;
    const std::vector<PieceOperateur*> piecesOperateur;
public:
    Niveau(int nbCol, int nbLigne, const std::vector<int>& levelData, const std::vector<PieceConcrete*>& piecesConcretes, const std::vector<PieceOperateur*>& piecesOperateur);
    // const int getData(int& x, int& y) const;
};

class AfficheurNiveau
{
private:
    const int MARGIN_LEFT = 100, MARGIN_RIGHT = 100, MARGIN_TOP = 100, MARGIN_BOTTOM = 100;
	const int TILE_SIZE = 64;
    const sf::Color COULEUR_DU_MUR = sf::Color{63,63,63};
    const sf::Color COULEUR_VIDE = sf::Color{0,0,0,0};

    sf::RenderWindow& fenetre;
	unsigned int nbPix_x;
	unsigned int nbPix_y;
    const std::vector<Niveau>& niveaux;
    int indiceNiveauActuel;
    int nbCol, nbLigne;

    sf::RectangleShape *const panneauCentral = new sf::RectangleShape;
    std::vector<sf::Vector2f> treillis;
    std::vector<sf::Vertex> sommetsTrame;
    std::vector<sf::Vertex> sommetsSceneParticuliere;

public:
    AfficheurNiveau(sf::RenderWindow& fenetre, std::vector<Niveau>& niveaux);
    void prochainNiveau();
    void allerAuNiveau(int indice);
    void dessiner();
    void demarrer();
private:
    void initialiseTrame();
    void genereTrellis();
    void ajouterSommetsCellule(std::vector<sf::Vertex>& trame, int& x, int& y, const sf::Color& couleur, int& indiceSommet);
    // const sf::Color& getCouleur(int data);
    // const sf::Vector2f& getPositionDeSommet(int x, int y) const;
};

#endif