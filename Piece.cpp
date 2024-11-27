#include "Piece.hpp"
#include <cmath>

//////////////////////////////
//////// CLASSE Piece ////////

Piece::Piece(const std::vector<pair<int, int>> &coords, const std::vector<PieceOperateur*> &operateurs, const CouleurPiece &couleur)
    : coordinates{std::move(coords)}, operateurs{std::move(operateurs)}, couleur{couleur}
{}

const std::vector<pair<int, int>>& Piece::getCoordinates() const {
    return coordinates;
}

OperateurDeplacement& Piece::ajouteOpDeplacement(const pair<int, int> &position, OperateurDeplacement::Orientation sens) {
    OperateurDeplacement* op = new OperateurDeplacement{std::move(position), sens};
    operateurs.push_back(op);
    return *op;
}
OperateurRotation& Piece::ajouteOpRotation(const pair<int, int> &position, OperateurRotation::Orientation sens) {
    OperateurRotation* op = new OperateurRotation{std::move(position), sens};
    operateurs.push_back(op);
    return *op;
}
OperateurSymetrie& Piece::ajouteOpSymetrie(const pair<int, int> &position, OperateurSymetrie::Orientation sens) {
    OperateurSymetrie* op = new OperateurSymetrie{std::move(position), sens};
    operateurs.push_back(op);
    return *op;
}

// void Piece::trigger(const pair<float, float> &pos) {
//     for (PieceOperateur *op : operateurs) {
//         if(op->trigger(pos)) return;
//     }
// }

void Piece::accept(PieceOperateur &op) {
    for (pair<int,int> &coord : coordinates) {
        op.mapPosition(coord);
    }
    for (PieceOperateur *otherOp : operateurs) {
        op.mapOperateur(*otherOp);
    }
}

/////////////////////////////////////
//////// CLASSE CouleurPiece ////////

CouleurPiece::CouleurPiece(const sf::Color &piece, const sf::Color &cible)
	: std::pair<const sf::Color, const sf::Color>{piece, cible}
{}

const CouleurPiece CouleurPiece::ROUGE {sf::Color{0xFD4030FF}, sf::Color{0xFEB0A9FF}};
const CouleurPiece CouleurPiece::ORANGE{sf::Color{0xFFAA00FF}, sf::Color{0xFFE1A8FF}};
const CouleurPiece CouleurPiece::VERT  {sf::Color{0x7AE300FF}, sf::Color{0xDBFFB3FF}};
const CouleurPiece CouleurPiece::CYAN  {sf::Color{0x20C3DFFF}, sf::Color{0xB9F4F4FF}};
const CouleurPiece CouleurPiece::BLEU  {sf::Color{0x2779FCFF}, sf::Color{0xB4D0FEFF}};
const CouleurPiece CouleurPiece::VIOLET{sf::Color{0xBC39D6FF}, sf::Color{0xE5B3EFFF}};