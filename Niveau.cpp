#include "Niveau.hpp"

///////////////////////////////
//////// CLASSE Niveau ////////

Niveau::Niveau(int nbCol, int nbLigne, const std::vector<int>& levelData, const std::vector<PieceConcrete*>& piecesConcretes, const std::vector<PieceOperateur*>& piecesOperateur)
	: nbCol{nbCol}, nbLigne{nbLigne}, levelData{levelData}, piecesConcretes{piecesConcretes}, piecesOperateur{piecesOperateur}
{}

// 

// const int Niveau::getData(int& x, int& y) const
// {
//     return levelData[y*nbCol + x];
// }

////////////////////////////////////////
//////// CLASSE AfficheurNiveau ////////

//////// CONSTRUCTEURS ////////

AfficheurNiveau::AfficheurNiveau(sf::RenderWindow& fenetre, std::vector<Niveau>& niveaux)
	: fenetre{fenetre}, niveaux{niveaux}
{
    allerAuNiveau(0);
}

//////// METHODES PUBLICS ////////

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

void AfficheurNiveau::dessiner()
{
	// Scène générale (le sol et les murs)
	// le sol
	fenetre.draw(*panneauCentral);
	// les murs
    fenetre.draw(&sommetsTrame[0], sommetsTrame.size(), sf::Triangles);

	// Scène particulière (les pièces concrètes)
	int indiceSommet = 0;
	for (PieceConcrete* p : niveaux[indiceNiveauActuel].piecesConcretes) {
		for (std::pair<int,int> coord : p->coordinates) {
			ajouterSommetsCellule(sommetsSceneParticuliere, coord.first, coord.second, p->couleur, indiceSommet);
		}
	}
    fenetre.draw(&sommetsSceneParticuliere[0], sommetsSceneParticuliere.size(), sf::Triangles);
}

void AfficheurNiveau::demarrer()
{
	std::vector<sf::Drawable*> scene_particuliere;

	// on place ici la définition d'une texture rouge, probablement qu'il y a mieux à faire (flightweight ? map ?)
   	sf::Texture texture_rouge;
	if (!texture_rouge.loadFromFile("texture.jpg"))
	{
		throw std::runtime_error("Erreur lors du chargement de l'image");
	}

	// Opérations graphiques générales
	while (fenetre.isOpen())
	{
		int trig_x = -1, trig_y = -1;
		sf::Vector2i mousePos = sf::Mouse::getPosition(fenetre);
		sf::Vector2f mouseWorldPos = fenetre.mapPixelToCoords(mousePos);
		std::string message = "Mouse Position: ("
							+ std::to_string(int(mouseWorldPos.x)) + ", "
							+ std::to_string(int(mouseWorldPos.y)) + ")";
		if (panneauCentral->getGlobalBounds().contains(mouseWorldPos))
		{
			sf::Vector2f topLeft = panneauCentral->getPosition();
			trig_x = (mouseWorldPos.x - topLeft.x) / TILE_SIZE;
			trig_y = (mouseWorldPos.y - topLeft.y) / TILE_SIZE;
			message += " case :" + std::to_string(trig_x) + " ; " + std::to_string(trig_y);
		}
		fenetre.setTitle(message);
		
		sf::Event event;
		while (fenetre.pollEvent(event))
		{
			if (event.type == sf::Event::Closed
				|| (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				fenetre.close();
			}
			if (event.type == sf::Event::MouseButtonPressed 
				&& event.mouseButton.button == sf::Mouse::Left 
				&& panneauCentral->getGlobalBounds().contains(mouseWorldPos))
			{
				std::cout << "trigger " << trig_x << " " << trig_y << std::endl;
				// création d'un sprite pour afficher une case d'exemple
				sf::Sprite* sprite = new sf::Sprite;
				sprite->setTexture(texture_rouge);
				sf::Vector2u textureSize = texture_rouge.getSize();
				sprite->setScale(static_cast<float>(TILE_SIZE) / textureSize.x, 
								 static_cast<float>(TILE_SIZE) / textureSize.y);
				sprite->setPosition(panneauCentral->getPosition()
				                    + sf::Vector2f(trig_x * TILE_SIZE, trig_y * TILE_SIZE)); 
				scene_particuliere.push_back(sprite);
			}
        }
        	
		fenetre.clear();
		fenetre.setView(fenetre.getDefaultView());

		// les affichages
		dessiner();
		for (sf::Drawable *x : scene_particuliere) fenetre.draw(*x);
		
        fenetre.display();
    }
}

//////// METHODES PRIVEES ////////

void AfficheurNiveau::initialiseTrame()
{
	// Le sol
    *panneauCentral = sf::RectangleShape{sf::Vector2f(nbCol * TILE_SIZE, nbLigne * TILE_SIZE)};
	panneauCentral->setPosition(MARGIN_LEFT, MARGIN_TOP);
	panneauCentral->setFillColor(sf::Color::Cyan);

    // Les murs
	genereTrellis();
	sommetsTrame.resize(nbCol * nbLigne * 6);
	int indiceSommet = 0;
	size_t indiceCellule = 0;
	for (int y = 0; y < nbLigne; ++y) {
		for (int x = 0; x < nbCol; ++x) {
			if (niveaux[indiceNiveauActuel].levelData[indiceCellule] == 1) {
				ajouterSommetsCellule(sommetsTrame, x, y, COULEUR_DU_MUR, indiceSommet);
			}
			++indiceCellule;
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

void AfficheurNiveau::ajouterSommetsCellule(std::vector<sf::Vertex>& trame, int& x, int& y, const sf::Color& couleur, int& indiceSommet)
{
	int indiceCellulePlusY{y*(nbCol+1) + x};
	trame[indiceSommet++] = sf::Vertex(treillis[indiceCellulePlusY], couleur);
	trame[indiceSommet++] = sf::Vertex(treillis[indiceCellulePlusY+1], couleur);
	trame[indiceSommet++] = sf::Vertex(treillis[indiceCellulePlusY+nbCol+1], couleur);
	trame[indiceSommet++] = sf::Vertex(treillis[indiceCellulePlusY+1], couleur);
	trame[indiceSommet++] = sf::Vertex(treillis[indiceCellulePlusY+nbCol+1], couleur);
	trame[indiceSommet++] = sf::Vertex(treillis[indiceCellulePlusY+nbCol+2], couleur);
}

// const sf::Color& AfficheurNiveau::getCouleur(int data)
// {
// 	switch (data) {
// 		case 1: return COULEUR_DU_MUR;
// 		case 0: return COULEUR_VIDE;
// 		default: return COULEUR_VIDE;
// 	}
// }

// const sf::Vector2f& AfficheurNiveau::getPositionDeSommet(int x, int y) const
// {
//     return treillis[y*(nbCol+1) + x];
// }