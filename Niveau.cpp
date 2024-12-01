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

PieceData& NiveauData::ajoutePiece(const std::vector<pair<int, int>> &coords, const CouleurPiece &couleur) {
	dataPieces.emplace_back(PieceData{coords, couleur});
	return dataPieces.back();
}

///////////////////////////////
//////// CLASSE Niveau ////////

//////// ATTRIBUTS STATIC CONST ////////

const sf::Color Niveau::COULEUR_DU_MUR = sf::Color{0x3F3F3FFF};
const sf::Color Niveau::COULEUR_DU_SOL = sf::Color{0xFFFFFCFF};

//////// CONSTRUCTEURS ////////

Niveau::Niveau(const NiveauData &dataNiveau)
	: NiveauData{dataNiveau}, pieces{}, casesActuelles{}, nbCasesOccupees{0},
		panneauCentral{sf::RectangleShape(sf::Vector2f(nbCol * TILE_SIZE, nbLigne * TILE_SIZE))},
		treillis{}, sommetsTrame{}
{
	panneauCentral.setPosition(MARGIN_LEFT, MARGIN_TOP);
	panneauCentral.setFillColor(COULEUR_DU_SOL);
	genereTreillis();

	pieces.reserve(dataPieces.size());
	for (const PieceData &dataPiece : dataPieces) {
		// pieces.emplace_back(Piece(static_cast<int>(pieces.size()), dataPiece, aff));
		pieces.emplace_back(Piece(static_cast<int>(pieces.size()), dataPiece, {}));
	}

	sommetsTrame.reserve(nbCol * nbLigne * 6);
	int indiceCellule = 0;
	for (int y = 0; y < nbLigne; ++y) {
		for (int x = 0; x < nbCol; ++x) {
			int dataAttendue = casesAttendues[indiceCellule];
			switch (dataAttendue) {
				case 0 :
					break;
				case 1 :
					ajouteSommetsCellule(sommetsTrame, x, y, COULEUR_DU_MUR);
					break;
				default	:
					int indicePiece = dataAttendue - 2;
					if (indicePiece < static_cast<int>(pieces.size()))
						ajouteSommetsCellule(sommetsTrame, x, y, pieces[indicePiece].getCouleurSecondaire());
					break;
			}
			++indiceCellule;
		}
	}

	casesActuelles.reserve(casesAttendues.size());
	// Recopier les positions des murs de `casesAttendues` dans `casesActuelles`
	for (int dataCase : casesAttendues) {
		casesActuelles.emplace_back((dataCase == 1) ? 1 : 0);
	}

	// Écriver les positions des `Piece` dans `casesActuelles`
	for (Piece &piece : pieces) {
		piece.sommets.reserve(piece.getCoordinates().size() * 6);
		for (const std::pair<int, int> &coord : piece.getCoordinates()) {
			setData(coord.first, coord.second, piece.getIndice() + 2);
			ajouteSommetsCellule(piece.sommets, coord.first, coord.second, piece.getCouleur());
		}
	}
}

//////// METHODES PUBLICS ////////

// const int Niveau::getNbCol() const { return nbCol; }
// const int Niveau::getNbLigne() const { return nbLigne; }
// const int Niveau::getNbPieces() const { return static_cast<int>(pieces.size()); }
// const int Niveau::getNbCasesOccupees() const { return nbCasesOccupees; }

// const int Niveau::getDataActuelleParIndice(int indice) const { return casesActuelles[indice]; }
const int Niveau::getDataActuelle(int x, int y) const { return casesActuelles[y * nbCol + x]; }
// const int Niveau::getDataAttendueParIndice(int indice) const { return casesAttendues[indice]; }
const int Niveau::getDataAttendue(int x, int y) const { return casesAttendues[y * nbCol + x]; }

// const sf::Color& Niveau::getCouleur(int indicePiece) const {
// 	return pieces[indicePiece].getCouleur();
// }
// const sf::Color& Niveau::getCouleurSecondaire(int indicePiece) const {
// 	return pieces[indicePiece].getCouleurSecondaire();
// }

void Niveau::setData(int x, int y, int value) {
	casesActuelles[y * nbCol + x] = value;
}

void Niveau::triggerPiece(int indice, int coordX, int coordY) {
	pieces[indice].trigger(coordX, coordY, casesActuelles);

	// TODO
	pieces[indice].sommets.clear();
	for (const std::pair<int, int> &coord : pieces[indice].getCoordinates()) {
		setData(coord.first, coord.second, indice + 2);
		ajouteSommetsCellule(pieces[indice].sommets, coord.first, coord.second, pieces[indice].getCouleur());
	}
}

void Niveau::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(panneauCentral);
    target.draw(&sommetsTrame[0], sommetsTrame.size(), sf::Triangles);
	for (const Piece &piece : pieces) {
		target.draw(piece);
	}
}


//////// METHODES PRIVEES ////////

void Niveau::genereTreillis()
{
	std::vector<sf::Vector2f>{}.swap(treillis);
	treillis.reserve((nbCol + 1) * (nbLigne + 1));
	for (int y = MARGIN_TOP; y <= MARGIN_TOP + nbLigne * TILE_SIZE; y += TILE_SIZE) {
		for (int x = MARGIN_LEFT; x <= MARGIN_LEFT + nbCol * TILE_SIZE; x += TILE_SIZE) {
			treillis.emplace_back(sf::Vector2f{static_cast<float>(x), static_cast<float>(y)});
		}
	}
}

void Niveau::ajouteSommetsCellule(std::vector<sf::Vertex>& trame, int x, int y, const sf::Color& couleur)
{
	int indiceCellulePlusY{y*(nbCol+1) + x};
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+2], couleur));
}

////////////////////////////////////////
//////// CLASSE AfficheurNiveau ////////

//////// CONSTRUCTEURS ////////

AfficheurNiveau::AfficheurNiveau(sf::RenderWindow &fenetre, const std::vector<NiveauData> &niveaux)
	// ici les deep-copies sont faites pour les `std::vector`
	// TODO : éviter la copie de `std::vector<NiveauData>& niveaux` ici...
	: fenetre{fenetre}, niveaux{niveaux},
		indiceNiveauActuel{0}, niveauActuel{Niveau(niveaux[0])}
{}

//////// METHODES PUBLICS ////////

void AfficheurNiveau::prochainNiveau()
{
    allerAuNiveau(indiceNiveauActuel + 1);
}

void AfficheurNiveau::allerAuNiveau(int indice)
{
    indiceNiveauActuel = indice;
	niveauActuel = Niveau(niveaux[indiceNiveauActuel]);
}

void AfficheurNiveau::demarrer()
{

	// Opérations graphiques générales
	while (fenetre.isOpen())
	{
		int coordX{-1};
		int coordY{-1};
		sf::Vector2i mousePos = sf::Mouse::getPosition(fenetre);
		sf::Vector2f mouseWorldPos = fenetre.mapPixelToCoords(mousePos);
		std::string message = "Mouse Position: ("
							+ std::to_string(int(mouseWorldPos.x)) + ", "
							+ std::to_string(int(mouseWorldPos.y)) + ")";
		bool sourisSurNiveau{niveauActuel.panneauCentral.getGlobalBounds().contains(mouseWorldPos)};
		if (sourisSurNiveau)
		{
			sf::Vector2f topLeft = niveauActuel.panneauCentral.getPosition(); // TODO
			coordX = static_cast<int>((mouseWorldPos.x - topLeft.x) / TILE_SIZE);
			coordY = static_cast<int>((mouseWorldPos.y - topLeft.y) / TILE_SIZE);
			message += " case :" + std::to_string(coordX) + " ; " + std::to_string(coordY);
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
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left 
				&& sourisSurNiveau)
			{
				std::cout << "trigger " << coordX << " " << coordY << std::endl;
				int indicePiece = niveauActuel.getDataActuelle(coordX, coordY) - 2;
				if (indicePiece >= 0)
				{
					niveauActuel.triggerPiece(indicePiece, coordX, coordY);
				}
			}
        }
        	
		fenetre.clear();
		fenetre.setView(fenetre.getDefaultView());

		// les affichages
		fenetre.draw(niveauActuel);
		
        fenetre.display();
    }
}
