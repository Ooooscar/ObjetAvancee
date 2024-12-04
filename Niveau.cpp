#include "Niveau.hpp"
#include <cassert>

///////////////////////////////////
//////// CLASSE NiveauData ////////

NiveauData::NiveauData(const int nbCol, const int nbLigne, const std::vector<int> &dataCasesAttendue) :
	nbCol{nbCol},
	nbLigne{nbLigne},
	dataCasesAttendue{dataCasesAttendue},
	dataPieces{}
{}
NiveauData::NiveauData(const int nbCol, const int nbLigne, std::vector<int> &&dataCasesAttendue) :
	nbCol{nbCol},
	nbLigne{nbLigne},
	dataCasesAttendue{dataCasesAttendue},
	dataPieces{}
{}

void NiveauData::ajouterPiece(const std::vector<std::pair<int, int>> &coords, const CouleurPiece &couleur, std::initializer_list<PieceOperateur*> operateurs) {
	dataPieces.emplace_back(PieceData{coords, couleur, operateurs});
}

///////////////////////////////
//////// CLASSE Niveau ////////

//////// ATTRIBUTS STATIC CONST ////////

const sf::Color Niveau::COULEUR_DU_MUR = sf::Color{0x3F3F3FFF};
const sf::Color Niveau::COULEUR_DU_SOL = sf::Color{0xFFFFFCFF};

//////// CONSTRUCTEURS ////////

Niveau::Niveau(const NiveauData &dataNiveau, const sf::Vector2f& coordCentre, float tailleCase) :
	NiveauData{dataNiveau},
	pieces{},
	dataCasesActuelle{},
	tailleCase{tailleCase},
	panneauCentral{sf::RectangleShape(sf::Vector2f(nbCol * tailleCase, nbLigne * tailleCase))},
	treillis{},
	sommetsTrame{}
{
	panneauCentral.setPosition(coordCentre - panneauCentral.getSize() / 2.0f);
	panneauCentral.setFillColor(COULEUR_DU_SOL);

	pieces.reserve(dataPieces.size());
	for (const PieceData &dataPiece : dataPieces) {
		pieces.emplace_back(Piece(*this, static_cast<int>(pieces.size()), dataPiece));
	}

	dataCasesActuelle.reserve(dataCasesAttendue.size());
	// Recopier les positions des murs de `dataCasesAttendue` dans `dataCasesActuelle`
	for (int dataCase : dataCasesAttendue) {
		dataCasesActuelle.emplace_back((dataCase == 1) ? 1 : 0);
	}

	treillis.reserve((nbCol + 1) * (nbLigne + 1));
	float margeEnHaut = panneauCentral.getPosition().y;
	float margeEnGauche = panneauCentral.getPosition().x;
	for (float y = margeEnHaut; y <= margeEnHaut + nbLigne * tailleCase; y += tailleCase) {
		for (float x = margeEnGauche; x <= margeEnGauche + nbCol * tailleCase; x += tailleCase) {
			treillis.emplace_back(sf::Vector2f{static_cast<float>(x), static_cast<float>(y)});
		}
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
	
	for (Piece &piece : pieces) {
		piece.sommets.reserve(piece.getCoordonnees().size() * 6);
		for (const std::pair<int, int> &caseOccupee : piece.getCoordonnees()){
			ajouterSommetsCellule(piece.sommets, caseOccupee.first, caseOccupee.second, piece.getCouleur());

			// Écriver les positions des `Piece` dans `dataCasesActuelle`
			redefinirData(caseOccupee, piece.getIndice() + 2);
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
	return std::make_pair<int>( static_cast<int>((posSouris.x - topLeft.x) / tailleCase),
		                        static_cast<int>((posSouris.y - topLeft.y) / tailleCase) );
}

void Niveau::redefinirData(const std::pair<int, int> &caseChoisie, int valeur) {
	dataCasesActuelle[caseChoisie.second * nbCol + caseChoisie.first] = valeur;
}

bool Niveau::triggerPiece(int indicePiece, const std::pair<int, int> &caseChoisie) {
	return pieces[indicePiece].trigger(caseChoisie);
}

const bool Niveau::estGagne() const {
	for (const Piece &piece : pieces) {
		if (!piece.estAuBonEndroit())
			return false;
	}
	return true;
}

void Niveau::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(panneauCentral);
    target.draw(&sommetsTrame[0], sommetsTrame.size(), sf::Triangles);
	for (const Piece &piece : pieces) {
		target.draw(piece);
	}
}


//////// METHODES PRIVEES ////////

void Niveau::ajouterSommetsCellule(std::vector<sf::Vertex>& trame, int x, int y, const sf::Color& couleur) {
	int indiceCellulePlusY{y*(nbCol+1) + x};
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+1], couleur));
	trame.emplace_back(sf::Vertex(treillis[indiceCellulePlusY+nbCol+2], couleur));
}
