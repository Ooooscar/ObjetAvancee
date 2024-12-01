#include "Niveau.hpp"
#include <cassert>

///////////////////////////////
//////// CLASSE Niveau ////////

Niveau::Niveau(const int nbCol, const int nbLigne, const std::vector<int> &&levelData)
	: nbCol{nbCol}, nbLigne{nbLigne}, levelData{levelData}, pieces{}
{}

Piece& Niveau::ajoutePiece(const std::vector<pair<int, int>> &coords, const CouleurPiece &couleur) {
	pieces.emplace_back(Piece{static_cast<int>(pieces.size()), coords, couleur});
	return pieces.back();
}

////////////////////////////////////////
//////// CLASSE AfficheurNiveau ////////

//////// ATTRIBUTS STATIC CONST ////////

const sf::Color AfficheurNiveau::COULEUR_DU_MUR = sf::Color{0x3F3F3FFF};
const sf::Color AfficheurNiveau::COULEUR_DU_SOL = sf::Color{0xFFFFFCFF};

//////// CONSTRUCTEURS ////////

AfficheurNiveau::AfficheurNiveau(sf::RenderWindow &fenetre, const std::vector<Niveau> &niveaux)
	// ici les deep-copies sont faites pour les `std::vector`
	// TODO : éviter la copie de `std::vector<Niveau>& niveaux` ici...
	: fenetre{fenetre}, niveaux{niveaux}, indiceNiveauActuel{0},
	  	nbCol{niveaux[indiceNiveauActuel].nbCol}, nbLigne{niveaux[indiceNiveauActuel].nbLigne},
	  	levelData{niveaux[indiceNiveauActuel].levelData}, pieces{niveaux[indiceNiveauActuel].pieces}
{
	initialiseNiveau();
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
	// ici les deep-copies sont faites pour les `std::vector`
	levelData = niveaux[indiceNiveauActuel].levelData;
	pieces = niveaux[indiceNiveauActuel].pieces;
	initialiseNiveau();
}

void AfficheurNiveau::dessiner()
{
	// Scène générale (le sol et les murs)
	fenetre.draw(*panneauCentral);
    fenetre.draw(&sommetsTrame[0], sommetsTrame.size(), sf::Triangles);

	// Scène particulière (les pièces concrètes)
	sommetsSceneParticuliere.clear();
	for (Piece p : pieces) {
		for (std::pair<int,int> coord : p.coordinates) {
			ajouterSommetsCellule(sommetsSceneParticuliere, coord.first, coord.second, p.couleur.first);
		}
	}
    fenetre.draw(&sommetsSceneParticuliere[0], sommetsSceneParticuliere.size(), sf::Triangles);
}

void AfficheurNiveau::demarrer()
{
	// for (int i : dataActuel)
	// 	std::cout << i;
	// std::cout << std::endl;

	// Opérations graphiques générales
	while (fenetre.isOpen())
	{
		int mouseX = -1, mouseY = -1;
		sf::Vector2i mousePos = sf::Mouse::getPosition(fenetre);
		sf::Vector2f mouseWorldPos = fenetre.mapPixelToCoords(mousePos);
		std::string message = "Mouse Position: ("
							+ std::to_string(int(mouseWorldPos.x)) + ", "
							+ std::to_string(int(mouseWorldPos.y)) + ")";
		if (panneauCentral->getGlobalBounds().contains(mouseWorldPos))
		{
			sf::Vector2f topLeft = panneauCentral->getPosition();
			mouseX = static_cast<int>((mouseWorldPos.x - topLeft.x) / TILE_SIZE);
			mouseY = static_cast<int>((mouseWorldPos.y - topLeft.y) / TILE_SIZE);
			message += " case :" + std::to_string(mouseX) + " ; " + std::to_string(mouseY);
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
				std::cout << "trigger " << mouseX << " " << mouseY << std::endl;
				int indicePiece = dataActuel[mouseY * nbCol + mouseX] - 2;
				if (indicePiece >= 0)
				{
					pieces[indicePiece].trigger(mouseX, mouseY, dataActuel);
				}
			}
        }
        	
		fenetre.clear();
		fenetre.setView(fenetre.getDefaultView());

		// les affichages
		dessiner();
		
        fenetre.display();
    }
}

//////// METHODES PRIVEES ////////

void AfficheurNiveau::initialiseNiveau()
{
	// Initialiser les sommets de la scène générale (les murs et le sol)
    initialiseTrame();

	// Recopier les positions des murs de `levelData` dans `dataActuel`
	std::vector<int>{}.swap(dataActuel);
	dataActuel.reserve(levelData.size());
	for (int data : levelData) {
		dataActuel.emplace_back((data == 1) ? 1 : 0);
	}

	// Allouer la mémoire pour les sommets des pièces concrètes
	// et générer les données actuelles du niveau
	int numeroPiece = 2;
	size_t nbCasesOccupes = 0;
	for (Piece &p : pieces) {
		nbCasesOccupes += p.coordinates.size();
		for (std::pair<int, int> &coord : p.coordinates) {
			dataActuel[coord.second*nbCol + coord.first] = numeroPiece;
		}
	}
	sommetsSceneParticuliere = std::vector<sf::Vertex>(nbCasesOccupes * 6);
}

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
			switch (levelData[indiceCellule]) {
				case 0 :
					break;
				case 1 :
					ajouterSommetsCellule(sommetsTrame, x, y, COULEUR_DU_MUR);
					break;
				default	:
					unsigned int indicePiece = levelData[indiceCellule] - 2;
					if (indicePiece < pieces.size())
						ajouterSommetsCellule(sommetsTrame, x, y, pieces[indicePiece].couleur.second);
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
