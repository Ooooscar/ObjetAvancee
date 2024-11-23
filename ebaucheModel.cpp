#include "ebaucheModel.hpp"

// PieceConcrete

PieceConcrete::PieceConcrete(const vector<pair<int, int>> &coords, const sf::Color &couleur)
    : coordinates{coords}, couleur{couleur}
{}

const vector<pair<int, int>>& PieceConcrete::getCoordinates() const {
    return coordinates;
}

void PieceConcrete::accept(const PieceOperateur &v) { 
    v.visit(*this);
}

// PieceOperateur

PieceOperateur::PieceOperateur(Piece &source, const pair<int,int> &position)
    : source{source}, position{position}
{}

const vector<pair<int, int>>& PieceOperateur::getCoordinates() const {
    return source.getCoordinates();
}

void PieceOperateur::trigger(const pair<int,int> &relativePos, Piece &origin) {
    if (relativePos == position) origin.accept(*this);
    else source.trigger(relativePos, origin);
}

void PieceOperateur::visit(PieceConcrete &p) const { 
    for (pair<int,int> &x : p.coordinates) mapPosition(x);
}

// OperateurDeplacement

OperateurDeplacement::OperateurDeplacement(Piece &source, const pair<int,int> &position, OrientationDeplacement sens)
    : PieceOperateur{source, position}, sens{sens}
{}

void OperateurDeplacement::accept(const PieceOperateur &v) {
    v.visit(*this);
}

void OperateurDeplacement::visit(OperateurDeplacement &x) const {
    // le deplacement ne modifie pas le sens
    mapPosition(x.position);
    x.source.accept(*this); 
}

void OperateurDeplacement::mapPosition(pair<int, int> &pos) const {
    switch (sens) {
        case NORD : pos.second--; break;
        case SUD : pos.second++; break;
        case EST : pos.first++; break;
        case OUEST : pos.first--; break;
    }
}