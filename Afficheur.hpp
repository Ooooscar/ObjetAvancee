#ifndef _PIECEOUT_RENDERER
#define _PIECEOUT_RENDERER
#include "Niveau.hpp"
#include <SFML/Graphics.hpp>

class AfficheurNiveau
{
private:
    static const sf::Color COULEUR_DU_MUR;
    static const sf::Color COULEUR_DU_SOL;

    const std::vector<NiveauData> niveaux;
    sf::RenderWindow fenetre;
    sf::Vector2f coordOrigine;
    float tailleCase;

    int indiceNiveauActuel;
    Niveau niveauActuel;

public:
    AfficheurNiveau(const std::vector<NiveauData> &niveaux);
    void prochainNiveau();
    void allerAuNiveau(int indice);
    void demarrer();
};

#endif