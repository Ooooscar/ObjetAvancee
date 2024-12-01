#include "Niveau.hpp"
#include <cassert>

///////////////////////////////////
//////// CLASSE NiveauData ////////

NiveauData::NiveauData(const int nbCol, const int nbLigne, const std::vector<int> &dataCasesAttendue)
	: nbCol{nbCol}, nbLigne{nbLigne}, dataCasesAttendue{dataCasesAttendue}, dataPieces{}
{}
NiveauData::NiveauData(const int nbCol, const int nbLigne, std::vector<int> &&dataCasesAttendue)
	: nbCol{nbCol}, nbLigne{nbLigne}, dataCasesAttendue{dataCasesAttendue}, dataPieces{}
{}

PieceData& NiveauData::ajouterPiece(const std::vector<pair<int, int>> &coords, const CouleurPiece &couleur) {
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
	: NiveauData{dataNiveau}, pieces{}, dataCasesActuelle{}, nbCasesOccupees{0},
		panneauCentral{sf::RectangleShape(sf::Vector2f(nbCol * TILE_SIZE, nbLigne * TILE_SIZE))},
		treillis{}, sommetsTrame{}
{
	panneauCentral.setPosition(MARGIN_LEFT, MARGIN_TOP);
	panneauCentral.setFillColor(COULEUR_DU_SOL);
	genererTreillis();

	pieces.reserve(dataPieces.size());
	for (const PieceData &dataPiece : dataPieces) {
		pieces.emplace_back(Piece(static_cast<int>(pieces.size()), dataPiece, {}));
	}

	sommetsTrame.reserve(nbCol * nbLigne * 6);
	int indiceCellule = 0;
	for (int y = 0; y < nbLigne; ++y) {
		for (int x = 0; x < nbCol; ++x) {
			int getDataAttendue = dataCasesAttendue[indiceCellule];
			switch (getDataAttendue) {
				case 0 :
					break;
				case 1 :
					ajouterSommetsCellule(sommetsTrame, x, y, COULEUR_DU_MUR);
					break;
				default	:
					int indicePiece = getDataAttendue - 2;
					if (indicePiece < static_cast<int>(pieces.size()))
						ajouterSommetsCellule(sommetsTrame, x, y, pieces[indicePiece].getCouleurSecondaire());
					break;
			}
			++indiceCellule;
		}
	}

	dataCasesActuelle.reserve(dataCasesAttendue.size());
	// Recopier les positions des murs de `dataCasesAttendue` dans `dataCasesActuelle`
	for (int dataCase : dataCasesAttendue) {
		dataCasesActuelle.emplace_back((dataCase == 1) ? 1 : 0);
	}

	// Écriver les positions des `Piece` dans `dataCasesActuelle`
	for (Piece &piece : pieces) {
		piece.sommets.reserve(piece.getCoordonnees().size() * 6);
		for (const std::pair<int, int> &caseOccupee : piece.getCoordonnees()) {
			redefinirData(caseOccupee, piece.getIndice() + 2);
			ajouterSommetsCellule(piece.sommets, caseOccupee.first, caseOccupee.second, piece.getCouleur());
		}
	}
}

//////// METHODES PUBLICS ////////

const int Niveau::getDataActuelle(const std::pair<int, int> &caseChoisie) const {
	return dataCasesActuelle[caseChoisie.second * nbCol + caseChoisie.first];
}
const int Niveau::getDataAttendue(const std::pair<int, int> &caseChoisie) const {
	return dataCasesAttendue[caseChoisie.second * nbCol + caseChoisie.first];
}

const bool Niveau::contient(const sf::Vector2f& posSouris) const {
	return panneauCentral.getGlobalBounds().contains(posSouris);
}
std::pair<int, int> Niveau::mapPixelsEnCases(const sf::Vector2f& posSouris) const {
	sf::Vector2f topLeft = panneauCentral.getPosition();
	return std::make_pair<int>( static_cast<int>((posSouris.x - topLeft.x) / TILE_SIZE),
		                        static_cast<int>((posSouris.y - topLeft.y) / TILE_SIZE) );
}

void Niveau::redefinirData(const std::pair<int, int> &caseChoisie, int valeur) {
	dataCasesActuelle[caseChoisie.second * nbCol + caseChoisie.first] = valeur;
}

void Niveau::triggerPiece(int indice, const std::pair<int, int> &caseChoisie) {
	pieces[indice].trigger(caseChoisie, dataCasesActuelle);

	// TODO
	pieces[indice].sommets.clear();
	for (const std::pair<int, int> &caseOccupee : pieces[indice].getCoordonnees()) {
		redefinirData(caseOccupee, indice + 2);
		ajouterSommetsCellule(pieces[indice].sommets, caseOccupee.first, caseOccupee.second, pieces[indice].getCouleur());
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

void Niveau::genererTreillis() {
	std::vector<sf::Vector2f>{}.swap(treillis);
	treillis.reserve((nbCol + 1) * (nbLigne + 1));
	for (int y = MARGIN_TOP; y <= MARGIN_TOP + nbLigne * TILE_SIZE; y += TILE_SIZE) {
		for (int x = MARGIN_LEFT; x <= MARGIN_LEFT + nbCol * TILE_SIZE; x += TILE_SIZE) {
			treillis.emplace_back(sf::Vector2f{static_cast<float>(x), static_cast<float>(y)});
		}
	}
}

void Niveau::ajouterSommetsCellule(std::vector<sf::Vertex>& trame, int x, int y, const sf::Color& couleur) {
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

void AfficheurNiveau::prochainNiveau() {
    allerAuNiveau(indiceNiveauActuel + 1);
}
void AfficheurNiveau::allerAuNiveau(int indice) {
    indiceNiveauActuel = indice;
	niveauActuel = Niveau(niveaux[indiceNiveauActuel]);
}

void AfficheurNiveau::demarrer()
{
	// Opérations graphiques générales
	while (fenetre.isOpen())
	{
		std::pair<int, int> caseChoisie{-1, -1};
		sf::Vector2i posSourisAbsolue{sf::Mouse::getPosition(fenetre)};
		// important, si on redimensionne la fenêtre...
		sf::Vector2f posSouris{fenetre.mapPixelToCoords(posSourisAbsolue)};
		std::string message = "Mouse Position: ("
							+ std::to_string(int(posSouris.x)) + ", "
							+ std::to_string(int(posSouris.y)) + ")";
		bool sourisSurNiveau{niveauActuel.contient(posSouris)};
		if (sourisSurNiveau)
		{
			caseChoisie = niveauActuel.mapPixelsEnCases(posSouris);
			message += " case :" + std::to_string(caseChoisie.first) + " ; " + std::to_string(caseChoisie.second);
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
				std::cout << "trigger " << caseChoisie.first << " " << caseChoisie.second << std::endl;
				int indicePiece = niveauActuel.getDataActuelle(caseChoisie) - 2;
				if (indicePiece >= 0)
				{
					niveauActuel.triggerPiece(indicePiece, caseChoisie);
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
