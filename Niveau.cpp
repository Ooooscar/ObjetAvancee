#include "Niveau.hpp"
#include <cassert>

///////////////////////////////////
//////// CLASSE NiveauData ////////

NiveauData::NiveauData(const int nbCol, const int nbLigne, const std::vector<int> &casesAttendues)
	: nbCol{nbCol}, nbLigne{nbLigne}, casesAttendues{casesAttendues}, dataPieces{}
{}
NiveauData::NiveauData(const int nbCol, const int nbLigne, const std::vector<int> &&casesAttendues)
	: nbCol{nbCol}, nbLigne{nbLigne}, casesAttendues{casesAttendues}, dataPieces{}
{}

Piece& NiveauData::ajoutePiece(const std::vector<pair<int, int>> &coords, const CouleurPiece &couleur) {
	dataPieces.emplace_back(Piece{static_cast<int>(dataPieces.size()), coords, couleur});
	return dataPieces.back();
}

///////////////////////////////////
//////// CLASSE Niveau ////////

Niveau::Niveau(const NiveauData &niveauData)
	: NiveauData{niveauData}, pieces{dataPieces}, casesActuelles{}, nbCasesOccupees{0}
{
	casesActuelles.reserve(casesAttendues.size());
	// Recopier les positions des murs de `casesAttendues` dans `casesActuelles`
	for (int data : casesAttendues) {
		casesActuelles.emplace_back((data == 1) ? 1 : 0);
	}
	// Écriver les positions des `Piece` dans `casesActuelles`
	for (Piece &p : pieces) {
		for (std::pair<int, int> &coord : p.coordinates) {
			setData(coord.first, coord.second, p.indice + 2);
			++nbCasesOccupees;
		}
	}
}

const int Niveau::getNbCol() const { return nbCol; }
const int Niveau::getNbLigne() const { return nbLigne; }
const int Niveau::getNbPieces() const { return static_cast<int>(pieces.size()); }
const int Niveau::getNbCasesOccupees() const { return nbCasesOccupees; }

const int Niveau::getDataActuelleParIndice(int indice) const { return casesActuelles[indice]; }
const int Niveau::getDataActuelle(int x, int y) const { return getDataActuelleParIndice(y * nbCol + x); }
const int Niveau::getDataAttendueParIndice(int indice) const { return casesAttendues[indice]; }
const int Niveau::getDataAttendue(int x, int y) const { return getDataAttendueParIndice(y * nbCol + x); }

const sf::Color& Niveau::getCouleurPiece(int indicePiece) const {
	return pieces[indicePiece].couleur.first;
}
const sf::Color& Niveau::getCouleurPieceSecondaire(int indicePiece) const {
	return pieces[indicePiece].couleur.second;
}

void Niveau::setData(int x, int y, int value) {
	casesActuelles[y * nbCol + x] = value;
}

void Niveau::triggerPiece(int indice, int mouseX, int mouseY) {
	pieces[indice].trigger(mouseX, mouseY, casesActuelles);
}

////////////////////////////////////////
//////// CLASSE AfficheurNiveau ////////

//////// ATTRIBUTS STATIC CONST ////////

const sf::Color AfficheurNiveau::COULEUR_DU_MUR = sf::Color{0x3F3F3FFF};
const sf::Color AfficheurNiveau::COULEUR_DU_SOL = sf::Color{0xFFFFFCFF};

//////// CONSTRUCTEURS ////////

AfficheurNiveau::AfficheurNiveau(sf::RenderWindow &fenetre, const std::vector<NiveauData> &niveaux)
	// ici les deep-copies sont faites pour les `std::vector`
	// TODO : éviter la copie de `std::vector<NiveauData>& niveaux` ici...
	: fenetre{fenetre}, niveaux{niveaux},
		indiceNiveauActuel{0}, niveauActuel{Niveau(niveaux[0])},
		nbCol{niveauActuel.getNbCol()}, nbLigne{niveauActuel.getNbLigne()}
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
	niveauActuel = Niveau(niveaux[indiceNiveauActuel]);
	initialiseNiveau();
}

void AfficheurNiveau::dessiner()
{
	// Scène générale (le sol et les murs)
	fenetre.draw(*panneauCentral);
    fenetre.draw(&sommetsTrame[0], sommetsTrame.size(), sf::Triangles);

	// Scène particulière (les pièces concrètes)
	sommetsSceneParticuliere.clear();
	for (Piece p : niveauActuel.pieces) { // TODO: make pieces private
		for (std::pair<int,int> coord : p.coordinates) {
			ajouteSommetsCellule(sommetsSceneParticuliere, coord.first, coord.second, p.couleur.first);
		}
	}
    fenetre.draw(&sommetsSceneParticuliere[0], sommetsSceneParticuliere.size(), sf::Triangles);
}

void AfficheurNiveau::demarrer()
{
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
				int indicePiece = niveauActuel.getDataActuelle(mouseX, mouseY) - 2;
				if (indicePiece >= 0)
				{
					niveauActuel.triggerPiece(indicePiece, mouseX, mouseY);
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

	// Allouer la mémoire pour les sommets des pièces
	sommetsSceneParticuliere = std::vector<sf::Vertex>(niveauActuel.getNbCasesOccupees() * 6);
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
			int dataAttendue = niveauActuel.getDataAttendueParIndice(indiceCellule);
			switch (dataAttendue) {
				case 0 :
					break;
				case 1 :
					ajouteSommetsCellule(sommetsTrame, x, y, COULEUR_DU_MUR);
					break;
				default	:
					int indicePiece = dataAttendue - 2;
					if (indicePiece < niveauActuel.getNbPieces())
						ajouteSommetsCellule(sommetsTrame, x, y, niveauActuel.getCouleurPieceSecondaire(indicePiece));
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

void AfficheurNiveau::ajouteSommetsCellule(std::vector<sf::Vertex>& trame, int& x, int& y, const sf::Color& couleur)
{
	int indiceCellulePlusY{y*(nbCol+1) + x};
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+2], couleur));
}
