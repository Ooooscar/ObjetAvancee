#include "Niveau.hpp"

Niveau::Niveau(int nbCol, int nbLigne, std::vector<int>& levelData)
: nbCol{nbCol}, nbLigne{nbLigne}, levelData{levelData}
{}

const int Niveau::getData(int& x, int& y) const
{
    return levelData[y*nbCol + x];
}

const sf::Color& AfficheurNiveau::getCouleur(int data)
{
	switch (data) {
		case 1: return COULEUR_DU_MUR;
		case 0: return COULEUR_VIDE;
		default: return COULEUR_VIDE;
	}
}

AfficheurNiveau::AfficheurNiveau(sf::RenderWindow& fenetre, std::vector<Niveau*>& niveaux)
: fenetre{fenetre}, niveaux{niveaux}
{
    allerAuNiveau(0);
}

void AfficheurNiveau::prochainNiveau()
{
    allerAuNiveau(indiceNiveauActuel + 1);
}
void AfficheurNiveau::allerAuNiveau(int indice)
{
    indiceNiveauActuel = indice;
    niveauActuel = niveaux[indiceNiveauActuel];
    nbCol = niveauActuel->nbCol;
    nbLigne = niveauActuel->nbLigne;
    initialiseTrame();
}

void AfficheurNiveau::dessiner()
{
    fenetre.draw(&sommetsTrame[0], sommetsTrame.size(), sf::Triangles);
}

void AfficheurNiveau::initialiseTrame()
{
    genereTrellis();
	sommetsTrame.resize(nbCol * nbLigne * 2 * 3);
	int indiceSommet = 0;
	int indiceCellule = 0;
	sf::Color couleurCellule;
	for (int y = 0; y < nbLigne; ++y) {
		for (int x = 0; x < nbCol; ++x) {
			couleurCellule = getCouleur(niveauActuel->getData(x, y));
			sommetsTrame[indiceSommet++] = sf::Vertex(getPositionDeSommet(x, y), couleurCellule);
			sommetsTrame[indiceSommet++] = sf::Vertex(getPositionDeSommet(x+1, y), couleurCellule);
			sommetsTrame[indiceSommet++] = sf::Vertex(getPositionDeSommet(x, y+1), couleurCellule);
			sommetsTrame[indiceSommet++] = sf::Vertex(getPositionDeSommet(x+1, y), couleurCellule);
			sommetsTrame[indiceSommet++] = sf::Vertex(getPositionDeSommet(x, y+1), couleurCellule);
			sommetsTrame[indiceSommet++] = sf::Vertex(getPositionDeSommet(x+1, y+1), couleurCellule);
			++indiceCellule;
		}
	}
}

void AfficheurNiveau::genereTrellis()
{
    treillis.reserve((niveauActuel->nbCol + 1) * (niveauActuel->nbLigne + 1));
	int indicePosition = 0;
	for (int y = MARGIN_TOP; y <= MARGIN_TOP + niveauActuel->nbLigne * TILE_SIZE; y += TILE_SIZE) {
		for (int x = MARGIN_LEFT; x <= MARGIN_LEFT + niveauActuel->nbCol * TILE_SIZE; x += TILE_SIZE) {
			treillis[indicePosition++] = sf::Vector2f{static_cast<float>(x), static_cast<float>(y)};
		}
	}
}

const sf::Vector2f& AfficheurNiveau::getPositionDeSommet(int x, int y) const
{
    return treillis[y*(nbCol+1) + x];
}