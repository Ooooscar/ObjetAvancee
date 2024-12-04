#ifndef _PIECEOUT_RENDERER
#define _PIECEOUT_RENDERER
#include "Niveau.hpp"
#include <SFML/Graphics.hpp>

class AfficheurNiveau
{
	enum Etat {ECRAN_PRINCIPAL, ECRAN_SELECTION, ECRAN_NIVEAU};

private:
    static const sf::Color COULEUR_DU_MUR;
    static const sf::Color COULEUR_DU_SOL;

    const std::vector<NiveauData> niveaux;
    sf::RenderWindow fenetre;
    sf::Vector2f coordOrigine;
    float tailleCase;

	Etat etat;

	sf::Font police;
	sf::Font policeSymboles;

    int indiceNiveauActuel;
    Niveau niveauActuel;

public:
    AfficheurNiveau(const std::vector<NiveauData> &niveaux);
	// void gererEvenement(const sf::Event& event);
	void chargerResources();

    void prochainNiveau();
    void allerAuNiveau(int indice);
    void demarrer();
};

#endif