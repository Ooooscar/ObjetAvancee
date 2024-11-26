#ifndef _PIECEOUT_LEVEL
#define _PIECEOUT_LEVEL
#include <utility>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "ebaucheModel.hpp"

class Niveau
{
public:
    const int nbCol;
    const int nbLigne;
    const std::vector<int> levelData;
    const std::vector<Piece*> pieces;
public:
    Niveau(const int nbCol, const int nbLigne, const std::vector<int> &levelData, const std::vector<Piece*> &pieces);
};

class AfficheurNiveau
{
private:
    static const int MARGIN_LEFT = 100, MARGIN_RIGHT = 100, MARGIN_TOP = 100, MARGIN_BOTTOM = 100;
	static const int TILE_SIZE = 96;
    static const sf::Color COULEUR_DU_MUR;
    static const sf::Color COULEUR_DU_SOL;

    sf::RenderWindow& fenetre;
    const std::vector<Niveau> niveaux;
    int indiceNiveauActuel;
    const Niveau *niveauActuel;
    int nbCol, nbLigne;
    std::vector<int> dataActuel;

    sf::RectangleShape *const panneauCentral = new sf::RectangleShape;
    std::vector<sf::Vector2f> treillis;
    std::vector<sf::Vertex> sommetsTrame;
    std::vector<sf::Vertex> sommetsSceneParticuliere;

public:
    AfficheurNiveau(sf::RenderWindow& fenetre, const std::vector<Niveau>& niveaux);
    void prochainNiveau();
    void allerAuNiveau(int indice);
    void dessiner();
    void demarrer();
private:
    void initialiseTrame();
    void genereTreillis();
    void ajouterSommetsCellule(std::vector<sf::Vertex>& trame, int& x, int& y, const sf::Color& couleur);
};

#endif