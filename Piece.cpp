#include "Piece.hpp"
#include <cmath>
#include <iostream>

//////////////////////////////////
//////// CLASSE PieceData ////////

PieceData::PieceData(const std::vector<pair<int, int>> &coords, const CouleurPiece &couleur)
    : coordinates{coords}, operateurs{}, couleur{couleur}
{}

OperateurDeplacement& PieceData::ajouteOpDeplacement(const pair<int, int> &position, OperateurDeplacement::Orientation sens) {
    OperateurDeplacement* op = new OperateurDeplacement{position, sens};
    operateurs.emplace_back(op);
    return *op;
}
OperateurRotation& PieceData::ajouteOpRotation(const pair<int, int> &position, OperateurRotation::Orientation sens) {
    OperateurRotation* op = new OperateurRotation{position, sens};
    operateurs.push_back(op);
    return *op;
}
OperateurSymetrie& PieceData::ajouteOpSymetrie(const pair<int, int> &position, OperateurSymetrie::Orientation sens) {
    OperateurSymetrie* op = new OperateurSymetrie{position, sens};
    operateurs.push_back(op);
    return *op;
}

//////////////////////////////
//////// CLASSE Piece ////////

// Piece::Piece(int indice, const PieceData &dataPiece, const AfficheurNiveau &aff)
//     : indice{indice}, PieceData{dataPiece}, sommets{}
// {
// }

Piece::Piece(int indice, const PieceData &dataPiece, const std::vector<sf::Vertex> &sommets)
    : PieceData{dataPiece}, indice{indice}, sommets{sommets}
{}

const int Piece::getIndice() const { return indice; }
const std::vector<pair<int, int>>& Piece::getCoordinates() const { return coordinates; }
const sf::Color& Piece::getCouleur() const { return couleur.first; }
const sf::Color& Piece::getCouleurSecondaire() const { return couleur.second; }

void Piece::trigger(int mouseX, int mouseY, std::vector<int> &casesActuelles) {
    for (PieceOperateur *op : operateurs) {
        if (mouseX == op->position.first && mouseY == op->position.second) {
            std::cout << "GREAT" << std::endl;
            accept(*op, casesActuelles);
        }
    }
}

void Piece::accept(PieceOperateur &op, std::vector<int> &casesActuelles) {
    int nbCol{8}; // TODO
    for (pair<int,int> &coord : coordinates) {
        casesActuelles[coord.second * nbCol + coord.first] = 0;
    }
    for (pair<int,int> &coord : coordinates) {
        op.mapPosition(coord);
        casesActuelles[coord.second * nbCol + coord.first] = indice + 2;
    }
    for (PieceOperateur *otherOp : operateurs) {
        op.mapOperateur(*otherOp);
    }
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