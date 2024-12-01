#ifndef _PIECEOUT_LEVEL
#define _PIECEOUT_LEVEL
#include <utility>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Piece.hpp"

class Niveau
{
public:
    const int nbCol;
    const int nbLigne;
    const std::vector<int> levelData;
    std::vector<Piece> pieces;
public:
    Niveau(const int nbCol, const int nbLigne, const std::vector<int> &&levelData);
    Piece& ajoutePiece(const std::vector<pair<int, int>> &coords, const CouleurPiece &couleur);
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
    int nbCol;
    int nbLigne;
    std::vector<int> levelData;
    std::vector<Piece> pieces;
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
    void initialiseNiveau();
    void initialiseTrame();
    void genereTreillis();
    void ajouterSommetsCellule(std::vector<sf::Vertex>& trame, int& x, int& y, const sf::Color& couleur);
};

#endif