#include "Niveau.hpp"

Niveau::Niveau(int nbCol, int nbLigne, const std::vector<int>& levelData)
: nbCol{nbCol}, nbLigne{nbLigne}, levelData{levelData}, piecesConcretes{}, piecesOperateur{}
{}

Niveau::Niveau(int nbCol, int nbLigne, const std::vector<int>& levelData, const std::vector<PieceConcrete*>& piecesConcretes, const std::vector<PieceOperateur*>& piecesOperateur)
: nbCol{nbCol}, nbLigne{nbLigne}, levelData{levelData}, piecesConcretes{piecesConcretes}, piecesOperateur{piecesOperateur}
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

AfficheurNiveau::AfficheurNiveau(sf::RenderWindow& fenetre, std::vector<Niveau>& niveaux)
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
    nbCol = niveaux[indiceNiveauActuel].nbCol;
    nbLigne = niveaux[indiceNiveauActuel].nbLigne;
	// Initialise les sommets de la scène générale (les murs et le sol)
    initialiseTrame();
	// Allouer la mémoire pour les sommets des pièces concrètes
	size_t nbCasesOccupes = 0;
	for (PieceConcrete* p : niveaux[indiceNiveauActuel].piecesConcretes) {
		nbCasesOccupes += p->getCoordinates().size();
	}
	sommetsSceneParticuliere = std::vector<sf::Vertex>(nbCasesOccupes * 6);
}

void AfficheurNiveau::initialiseTrame()
{
    genereTrellis();
	sommetsTrame.resize(nbCol * nbLigne * 6);
	int indiceSommet = 0;
	const sf::Color *couleurCellule;
	for (int y = 0; y < nbLigne; ++y) {
		for (int x = 0; x < nbCol; ++x) {
			couleurCellule = &getCouleur(niveaux[indiceNiveauActuel].getData(x, y));
			sommetsTrame[indiceSommet++] = sf::Vertex(getPositionDeSommet(x, y), *couleurCellule);
			sommetsTrame[indiceSommet++] = sf::Vertex(getPositionDeSommet(x+1, y), *couleurCellule);
			sommetsTrame[indiceSommet++] = sf::Vertex(getPositionDeSommet(x, y+1), *couleurCellule);
			sommetsTrame[indiceSommet++] = sf::Vertex(getPositionDeSommet(x+1, y), *couleurCellule);
			sommetsTrame[indiceSommet++] = sf::Vertex(getPositionDeSommet(x, y+1), *couleurCellule);
			sommetsTrame[indiceSommet++] = sf::Vertex(getPositionDeSommet(x+1, y+1), *couleurCellule);
		}
	}
}

void AfficheurNiveau::genereTrellis()
{
    treillis.reserve((nbCol + 1) * (nbLigne + 1));
	int indicePosition = 0;
	for (int y = MARGIN_TOP; y <= MARGIN_TOP + nbLigne * TILE_SIZE; y += TILE_SIZE) {
		for (int x = MARGIN_LEFT; x <= MARGIN_LEFT + nbCol * TILE_SIZE; x += TILE_SIZE) {
			treillis[indicePosition++] = sf::Vector2f{static_cast<float>(x), static_cast<float>(y)};
		}
	}
}

const sf::Vector2f& AfficheurNiveau::getPositionDeSommet(int x, int y) const
{
    return treillis[y*(nbCol+1) + x];
}

void AfficheurNiveau::dessiner()
{
	// Scène générale (les murs et le sol)
    fenetre.draw(&sommetsTrame[0], sommetsTrame.size(), sf::Triangles);
	// Scène particulière (les pièces concrètes)
	int indiceSommet = 0;
	const sf::Color *couleurCellule;
	int x, y;
	for (PieceConcrete* p : niveaux[indiceNiveauActuel].piecesConcretes) {
		for (std::pair<int,int> coord : p->coordinates) {
			x = coord.first; y = coord.second;
			couleurCellule = &(p->couleur);
			sommetsSceneParticuliere[indiceSommet++] = sf::Vertex(getPositionDeSommet(x, y), *couleurCellule);
			sommetsSceneParticuliere[indiceSommet++] = sf::Vertex(getPositionDeSommet(x+1, y), *couleurCellule);
			sommetsSceneParticuliere[indiceSommet++] = sf::Vertex(getPositionDeSommet(x, y+1), *couleurCellule);
			sommetsSceneParticuliere[indiceSommet++] = sf::Vertex(getPositionDeSommet(x+1, y), *couleurCellule);
			sommetsSceneParticuliere[indiceSommet++] = sf::Vertex(getPositionDeSommet(x, y+1), *couleurCellule);
			sommetsSceneParticuliere[indiceSommet++] = sf::Vertex(getPositionDeSommet(x+1, y+1), *couleurCellule);
		}
	}
    fenetre.draw(&sommetsSceneParticuliere[0], sommetsSceneParticuliere.size(), sf::Triangles);
}