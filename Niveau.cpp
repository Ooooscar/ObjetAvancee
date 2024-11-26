#include "Niveau.hpp"
#include <cassert>

///////////////////////////////
//////// CLASSE Niveau ////////

Niveau::Niveau(const int nbCol, const int nbLigne, const std::vector<int> &levelData, const std::vector<Piece*>& pieces)
	: nbCol{nbCol}, nbLigne{nbLigne}, levelData{std::move(levelData)}, pieces{std::move(pieces)}
{}

////////////////////////////////////////
//////// CLASSE AfficheurNiveau ////////

//////// ATTRIBUTS STATIC CONST ////////

const sf::Color AfficheurNiveau::COULEUR_DU_MUR = sf::Color{0x3F3F3FFF};
const sf::Color AfficheurNiveau::COULEUR_DU_SOL = sf::Color{0xFFFFFCFF};

//////// CONSTRUCTEURS ////////

AfficheurNiveau::AfficheurNiveau(sf::RenderWindow& fenetre, const std::vector<Niveau>& niveaux)
	: fenetre{fenetre}, niveaux{std::move(niveaux)}
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
	niveauActuel = &(niveaux[indiceNiveauActuel]);
    nbCol = niveauActuel->nbCol;
    nbLigne = niveauActuel->nbLigne;

	// Initialise les sommets de la scène générale (les murs et le sol)
    initialiseTrame();

	// On veut recopier les données du niveau choisi ici
	dataActuel = std::vector<int>(niveauActuel->levelData.size());
	std::vector<int>::const_iterator itLecture{niveauActuel->levelData.begin()};
	std::vector<int>::iterator itEcriture{dataActuel.begin()};
	for (; itEcriture != dataActuel.end(); ++itLecture, ++itEcriture) {
		*itEcriture = (*itLecture == 1) ? 1 : 0;
	}
	dataActuel = niveauActuel->levelData; // ici une copie est faite
	for (size_t i = 0; i < dataActuel.size(); ++i) {
		dataActuel[i] = (niveauActuel->levelData[i] == 1) ? 1 : 0;
	}

	// Allouer la mémoire pour les sommets des pièces concrètes
	// et générer les données actuelles du niveau
	int numeroPiece = 2;
	size_t nbCasesOccupes = 0;
	for (Piece *p : niveauActuel->pieces) {
		nbCasesOccupes += p->coordinates.size();
		for (std::pair<int, int> &coord : p->coordinates) {
			dataActuel[coord.second*nbCol + coord.first] = numeroPiece;
		}
	}
	sommetsSceneParticuliere = std::vector<sf::Vertex>(nbCasesOccupes * 6);
}

void AfficheurNiveau::dessiner()
{
	// Scène générale (le sol et les murs)
	fenetre.draw(*panneauCentral);
    fenetre.draw(&sommetsTrame[0], sommetsTrame.size(), sf::Triangles);

	// Scène particulière (les pièces concrètes)
	for (Piece *p : niveaux[indiceNiveauActuel].pieces) {
		for (std::pair<int,int> coord : p->coordinates) {
			ajouterSommetsCellule(sommetsSceneParticuliere, coord.first, coord.second, p->couleur.first);
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
		float trig_x = -1, trig_y = -1;
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
	panneauCentral->setFillColor(COULEUR_DU_SOL);

    // Les murs
	genereTreillis();
	
	std::vector<sf::Vertex>{}.swap(sommetsTrame);
	sommetsTrame.reserve(nbCol * nbLigne * 6);
	int indiceCellule = 0;
	for (int y = 0; y < nbLigne; ++y) {
		for (int x = 0; x < nbCol; ++x) {
			switch (niveauActuel->levelData[indiceCellule]) {
				case 0 :
					break;
				case 1 :
					ajouterSommetsCellule(sommetsTrame, x, y, COULEUR_DU_MUR);
					break;
				default	:
					ajouterSommetsCellule(sommetsTrame, x, y, niveauActuel->pieces[0]->couleur.second);
					break;
			}
			++indiceCellule;
		}
	}
}

void AfficheurNiveau::genereTreillis()
{
	std::vector<sf::Vector2f>{}.swap(treillis);
	treillis.reserve((nbCol + 1) * (nbLigne + 1));
	for (int y = MARGIN_TOP; y <= MARGIN_TOP + nbLigne * TILE_SIZE; y += TILE_SIZE) {
		for (int x = MARGIN_LEFT; x <= MARGIN_LEFT + nbCol * TILE_SIZE; x += TILE_SIZE) {
			treillis.emplace_back(sf::Vector2f{static_cast<float>(x), static_cast<float>(y)});
		}
	}
}

void AfficheurNiveau::ajouterSommetsCellule(std::vector<sf::Vertex>& trame, int& x, int& y, const sf::Color& couleur)
{
	int indiceCellulePlusY{y*(nbCol+1) + x};
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+2], couleur));
}
