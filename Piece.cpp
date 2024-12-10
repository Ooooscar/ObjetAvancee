#include "Piece.hpp"
#include "Niveau.hpp"
#include <cmath>
#include <iostream>

//////////////////////////////////
//////// CLASSE PieceData ////////

PieceData::PieceData(const std::vector<std::pair<int, int>> &coords, const CouleurPiece &couleur)
    : coordonnees{coords}
    , operateurs{}
    , couleur{couleur}
{}

PieceData::PieceData(const PieceData& other)
    : coordonnees{other.coordonnees}
    , operateurs{}
    , couleur{other.couleur}
{
    operateurs.reserve(other.operateurs.size());
    for (const PieceOperateurData *const op : other.operateurs) {
        operateurs.push_back(op->clone());
    }
}

PieceData::~PieceData() {
    for (PieceOperateurData *op : operateurs) {
        delete op;
    }
}

const std::vector<std::pair<int, int>>& PieceData::getCoordonnees() const { return coordonnees; }
const sf::Color& PieceData::getCouleur() const { return couleur.first; }
const sf::Color& PieceData::getCouleurSecondaire() const { return couleur.second; }

void PieceData::ajouterOpDeplacement(const std::pair<int, int> &position, OperateurDeplacement::Orientation sens) {
    operateurs.emplace_back(new OperateurDeplacement{position, sens});
}
void PieceData::ajouterOpRotation(const std::pair<int, int> &position, OperateurRotation::Orientation sens) {
    operateurs.emplace_back(new OperateurRotation{position, sens});
}
void PieceData::ajouterOpSymetrie(const std::pair<int, int> &position, OperateurSymetrie::Orientation sens) {
    operateurs.emplace_back(new OperateurSymetrie{position, sens});
}

//////////////////////////////
//////// CLASSE Piece ////////

Piece::Piece(Niveau &niveau, int indicePiece, const PieceData &dataPiece)
    : PieceData{dataPiece}
    , niveau{niveau}
    , indicePiece{indicePiece}
    , sommets{}
    , auBonEndroit{false}
    , enMouvement{false}
{}

const int Piece::getIndice() const { return indicePiece; }
const bool Piece::estAuBonEndroit() const { return auBonEndroit; }

bool Piece::trigger(const std::pair<int, int> &caseChoisie) {
    for (PieceOperateurData *op : operateurs) {
        if (caseChoisie == op->getPosition()) {
            if (accepter(*op)) return true;
        }
    }
    return false;
}

bool Piece::accepter(PieceOperateurData &op) {
    for (std::pair<int, int> caseOccupee_copie : coordonnees) {
        op.mapPosition(caseOccupee_copie);
        int dataCible = niveau.getDataActuelle(caseOccupee_copie);
        if (dataCible && dataCible != indicePiece + 2) {
            // donc cette nouvelle case à visiter est occupé
            return false;
        }
    }
    // Ici alors l'operation à accepter est valide
    auBonEndroit = true;
    sommets.clear();
    for (const std::pair<int, int> &caseOccupee : coordonnees) {
        niveau.redefinirData(caseOccupee, 0);
    }
	for (std::pair<int, int> &caseOccupee : coordonnees) {
        op.mapPosition(caseOccupee);
        auBonEndroit &= (niveau.getDataAttendue(caseOccupee) == indicePiece + 2);
		niveau.redefinirData(caseOccupee, indicePiece + 2);
		niveau.ajouterSommetsCellule(sommets, caseOccupee.first, caseOccupee.second, getCouleur());
	}
    for (PieceOperateurData *otherOp : operateurs) {
        op.mapOperateur(*otherOp);
    }
    return true;
}

void Piece::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(&sommets[0], sommets.size(), sf::Triangles);
}

/////////////////////////////////////
//////// CLASSE CouleurPiece ////////

CouleurPiece::CouleurPiece(const sf::Color &piece, const sf::Color &cible)
    : std::pair<sf::Color, sf::Color>{piece, cible}
{}

const CouleurPiece CouleurPiece::ROUGE {sf::Color{0xFD4030FF}, sf::Color{0xFEB0A9FF}};
const CouleurPiece CouleurPiece::ORANGE{sf::Color{0xFFAA00FF}, sf::Color{0xFFE1A8FF}};
const CouleurPiece CouleurPiece::VERT  {sf::Color{0x7AE300FF}, sf::Color{0xDBFFB3FF}};
const CouleurPiece CouleurPiece::CYAN  {sf::Color{0x20C3DFFF}, sf::Color{0xB9F4F4FF}};
const CouleurPiece CouleurPiece::BLEU  {sf::Color{0x2779FCFF}, sf::Color{0xB4D0FEFF}};
const CouleurPiece CouleurPiece::VIOLET{sf::Color{0xBC39D6FF}, sf::Color{0xE5B3EFFF}};