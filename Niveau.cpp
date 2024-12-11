#include "Niveau.hpp"
#include <cassert>

///////////////////////////////////
//////// CLASSE NiveauData ////////

NiveauData::NiveauData( const int nbCol, const int nbLigne,
						std::vector<int> &&casesAttendues, std::vector<int> &&casesActuelles,
						std::initializer_list<PieceOperateurData*> operateurs,
						std::initializer_list<CouleurPiece> couleurs )
	: nbCol{nbCol}
	, nbLigne{nbLigne}
	, dataCasesAttendue{casesAttendues}
	, dataCasesActuelle{casesActuelles}
	, dataOperateurs{operateurs}
	, couleurs{couleurs}
{}

const int NiveauData::getDataAttendue(const std::pair<int, int> &caseChoisie) const {
	return dataCasesAttendue[caseChoisie.second * nbCol + caseChoisie.first];
}
const int NiveauData::getDataActuelle(const std::pair<int, int> &caseChoisie) const {
	return dataCasesActuelle[caseChoisie.second * nbCol + caseChoisie.first];
}

void NiveauData::redefinirData(const std::pair<int, int> &caseChoisie, int valeur) {
	dataCasesActuelle[caseChoisie.second * nbCol + caseChoisie.first] = valeur;
}

///////////////////////////////
//////// CLASSE Niveau ////////

//////// ATTRIBUTS STATIC CONST ////////

const sf::Color Niveau::COULEUR_DU_MUR = sf::Color{0x3F3F3FFF};
const sf::Color Niveau::COULEUR_DU_SOL = sf::Color{0xFFFFFCFF};

//////// CONSTRUCTEURS ////////

Niveau::Niveau(const NiveauData &dataNiveau, const sf::Vector2f& coordCentre, float tailleCase)
	: NiveauData{dataNiveau}
	, pieces{}
	, minuterieAnimation{}
	, pieceEnMouvement{nullptr}
	, gagne{false}
	, tailleCase{tailleCase}
	, panneauCentral{sf::RectangleShape(sf::Vector2f(nbCol * tailleCase, nbLigne * tailleCase))}
	, sommetsTrame{}
{
	panneauCentral.setPosition(coordCentre - panneauCentral.getSize() / 2.0f);
	panneauCentral.setFillColor(COULEUR_DU_SOL);

	// treillis.reserve((nbCol + 1) * (nbLigne + 1));
	// float margeEnGauche = panneauCentral.getPosition().x;
	// float margeEnHaut = panneauCentral.getPosition().y;
	// for (float y = margeEnHaut; y <= margeEnHaut + nbLigne * tailleCase; y += tailleCase) {
	// 	for (float x = margeEnGauche; x <= margeEnGauche + nbCol * tailleCase; x += tailleCase) {
	// 		treillis.emplace_back(sf::Vector2f{x, y});
	// 	}
	// }

	pieces.reserve(couleurs.size());
	for (int indicePiece = 0; indicePiece < static_cast<int>(couleurs.size()); ++indicePiece) {
		pieces.emplace_back(*this, indicePiece, PieceData{{}, couleurs[indicePiece]});
	}

	sommetsTrame.reserve(nbCol * nbLigne * 6);
	int indiceCellule = 0;
	for (int y = 0; y < nbLigne; ++y) {
		for (int x = 0; x < nbCol; ++x) {
			int dataAttendue = dataCasesAttendue[indiceCellule];
			switch (dataAttendue) {
			case 0 :
				break;
			case 1 :
				ajouterSommetsCellule(sommetsTrame, x, y, COULEUR_DU_MUR);
				break;
			default	:
				int indicePiece = dataAttendue - 2;
				// if (indicePiece < static_cast<int>(pieces.size()))
					ajouterSommetsCellule(sommetsTrame, x, y, couleurs[indicePiece].second);
				break;
			}
			int dataActuelle = dataCasesActuelle[indiceCellule];
			int indicePiece = dataActuelle - 2;
			if (indicePiece >= 0) {
				// if (indicePiece < static_cast<int>(pieces.size()))
				pieces[indicePiece].coordonnees.emplace_back(std::make_pair(x, y));
			}
			++indiceCellule;
		}
	}
	
	for (Piece &piece : pieces) {
		piece.sommets.reserve(piece.getCoordonnees().size() * 6);
		for (const std::pair<int, int> &caseOccupee : piece.getCoordonnees()){
			ajouterSommetsCellule(piece.sommets, caseOccupee.first, caseOccupee.second, piece.getCouleur());
		}
	}

	for (PieceOperateurData *op : dataOperateurs) {
		int caseData = getDataActuelle(op->getPosition());
		int indicePiece = caseData - 2;
		// if (indicePiece >= 0 && indicePiece < pieces.size())
		pieces[indicePiece].operateurs.emplace_back(op->clone());
	}
}

//////// METHODES PUBLICS ////////

const bool Niveau::contient(const sf::Vector2f& posSouris) const {
	return panneauCentral.getGlobalBounds().contains(posSouris);
}
std::pair<int, int> Niveau::mapPixelsEnCases(const sf::Vector2f& posSouris) const {
	sf::Vector2f topLeft = panneauCentral.getPosition();
	return std::make_pair<int>( static_cast<int>((posSouris.x - topLeft.x) / tailleCase),
		                        static_cast<int>((posSouris.y - topLeft.y) / tailleCase) );
}

bool Niveau::triggerPiece(int indicePiece, const std::pair<int, int> &caseChoisie) {
	pieceEnMouvement = &pieces[indicePiece];
	minuterieAnimation.restart();
	return pieces[indicePiece].trigger(caseChoisie);
}

const bool Niveau::estEnMouvement() const {
	return pieceEnMouvement != nullptr;
}
const bool Niveau::estGagne() const {
	return gagne;
}
void Niveau::updateGagne() {
	for (const Piece &piece : pieces) {
		if (!piece.estAuBonEndroit()) {
			gagne = false;
			return;
		}
	}
	gagne = true;
}

void Niveau::updateAnimation() {
	pieceEnMouvement->update(minuterieAnimation.getElapsedTime());
}

void Niveau::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(panneauCentral);
    target.draw(&sommetsTrame[0], sommetsTrame.size(), sf::Triangles);
	for (const Piece &piece : pieces) {
		target.draw(piece);
	}
}


//////// METHODES PRIVEES ////////

// void Niveau::ajouterSommetsCellule(std::vector<sf::Vertex>& trame, int x, int y, const sf::Color& couleur) {
// 	int indiceCellulePlusY{y*(nbCol+1) + x};
// 	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY], couleur));
// 	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+1], couleur));
// 	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+1], couleur));
// 	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+1], couleur));
// 	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+1], couleur));
// 	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+2], couleur));
// }

void Niveau::ajouterSommetsCellule(std::vector<sf::Vertex>& trame, float x, float y, const sf::Color& couleur) {
	float margeEnGauche = panneauCentral.getPosition().x;
	float posX = margeEnGauche + x * tailleCase;
	float margeEnHaut = panneauCentral.getPosition().y;
	float posY = margeEnHaut + y * tailleCase;
	trame.emplace_back(sf::Vertex({posX, posY}, couleur));
	trame.emplace_back(sf::Vertex({posX + tailleCase, posY}, couleur));
	trame.emplace_back(sf::Vertex({posX, posY + tailleCase}, couleur));
	trame.emplace_back(sf::Vertex({posX + tailleCase, posY}, couleur));
	trame.emplace_back(sf::Vertex({posX, posY + tailleCase}, couleur));
	trame.emplace_back(sf::Vertex({posX + tailleCase, posY + tailleCase}, couleur));
}
