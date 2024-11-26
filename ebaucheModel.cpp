#include "ebaucheModel.hpp"
#include <cmath>

//////////////////////////////
//////// CLASSE Piece ////////

Piece::Piece(const vector<pair<int, int>> &coords, const vector<PieceOperateur*> &operateurs, const CouleurPiece &couleur)
    : coordinates{coords}, operateurs{operateurs}, couleur{couleur}
{}

const vector<pair<int, int>>& Piece::getCoordinates() const {
    return coordinates;
}

void Piece::trigger(const pair<float, float> &pos) {
    for (PieceOperateur *op : operateurs) {
        if(op->trigger(pos)) return;
    }
}

void Piece::accept(PieceOperateur &v) {
    for (pair<int,int> &coord : coordinates) v.mapPosition(coord);
    for (PieceOperateur *op : operateurs) op->accept(v);
}

///////////////////////////////////////
//////// CLASSE PieceOperateur ////////

PieceOperateur::PieceOperateur(Piece &source, const pair<int,int> &position)
    : source{source}, position{position}
{}

bool PieceOperateur::trigger(const pair<float, float> &pos) {
    if ((int)pos.first == position.first && (int)pos.second == position.second) {
        source.accept(*this);
        return true;
    }
    return false;
}

/////////////////////////////////////////////
//////// CLASSE OperateurDeplacement ////////

OperateurDeplacement::OperateurDeplacement(Piece &source, const pair<int,int> &position, OrientationDeplacement sens)
    : PieceOperateur{source, position}, sens{sens}
{}

void OperateurDeplacement::accept(const PieceOperateur &v) {
    v.mapPosition(position);
}

void OperateurDeplacement::mapPosition(pair<int, int> &pos) const {
    switch (sens) {
        case NORD : pos.second--; break;
        case SUD : pos.second++; break;
        case EST : pos.first++; break;
        case OUEST : pos.first--; break;
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