#include "PieceOperateur.hpp"
#define _USE_MATH_DEFINES //pour M_PI
#include <cmath>

///////////////////////////////////////////
//////// CLASSE PieceOperateurData ////////

PieceOperateurData::PieceOperateurData(const std::pair<int,int> &position)
    : position{position}
{}

const std::pair<int,int>& PieceOperateurData::getPosition() const {
    return position;
}

void PieceOperateurData::accepter(const OperateurDeplacement &op) {
    op.mapPosition(position);
}
void PieceOperateurData::accepter(const OperateurRotation &op) {
    op.mapPosition(position);
}
void PieceOperateurData::accepter(const OperateurSymetrie &op) {
    op.mapPosition(position);
}

/////////////////////////////////////////////
//////// CLASSE OperateurDeplacement ////////

OperateurDeplacement::OperateurDeplacement(const std::pair<int,int> &position, Orientation sens)
    : PieceOperateurData{position}
    , sens{sens}
{}

PieceOperateurData* OperateurDeplacement::clone() const {
    return new OperateurDeplacement{*this};
}
const OperateurDeplacement::Orientation OperateurDeplacement::getSens() const {
    return sens;
}

void OperateurDeplacement::mapPosition(std::pair<int, int> &pos) const {
    switch (sens) {
    case NORD : pos.second--; break;
    case SUD : pos.second++; break;
    case EST : pos.first++; break;
    case OUEST : pos.first--; break;
    }
}
void OperateurDeplacement::mapPosition(sf::Vector2f &pos, float t) const {
    switch (sens) {
    case NORD : pos.y -= t; break;
    case SUD : pos.y += t; break;
    case EST : pos.x += t; break;
    case OUEST : pos.x -= t; break;
    }
}
void OperateurDeplacement::mapOperateur(PieceOperateurData &op) const {
    op.accepter(*this);
}

void OperateurDeplacement::accepter(const OperateurRotation &op) {
    PieceOperateurData::accepter(op);
    switch (op.getSens()) {
    case OperateurRotation::HORAIRE :
        switch (sens) {
        case EST : sens = SUD; break;
        case SUD : sens = OUEST; break;
        case OUEST : sens = NORD; break;
        case NORD : sens = EST; break;
        }
        break;
    case OperateurRotation::ANTIHORAIRE :
        switch (sens) {
        case EST : sens = NORD; break;
        case SUD : sens = EST; break;
        case OUEST : sens = SUD; break;
        case NORD : sens = OUEST; break;
        }
        break;
    }
}
void OperateurDeplacement::accepter(const OperateurSymetrie &op) {
    PieceOperateurData::accepter(op);
    switch (op.getSens()) {
    case OperateurSymetrie::VERTICALE :
        switch (sens) {
        case EST : sens = OUEST; break;
        case OUEST : sens = EST; break;
        default : break;
        }
        break;
    case OperateurSymetrie::HORIZONTALE :
        switch (sens) {
        case NORD : sens = SUD; break;
        case SUD : sens = NORD; break;
        default : break;
        }
        break;
    }
}

//////////////////////////////////////////
//////// CLASSE OperateurRotation ////////

OperateurRotation::OperateurRotation(const std::pair<int,int> &position, Orientation sens)
    : PieceOperateurData{position}
    , sens{sens}
{}

PieceOperateurData* OperateurRotation::clone() const {
    return new OperateurRotation{*this};
}
const OperateurRotation::Orientation OperateurRotation::getSens() const {
    return sens;
}

void OperateurRotation::mapPosition(std::pair<int, int> &pos) const {
    int firstTmp;
    switch (sens) {
    case HORAIRE :
        // Rotation 90° horarire : (x, y) -> (y, -x)
        firstTmp = position.first - (pos.second - position.second);
        pos.second = position.second + (pos.first - position.first);
        break;
    case ANTIHORAIRE :
        // Rotation 90° anti-horarire : (x, y) -> (-y, x)
        firstTmp = position.first + (pos.second - position.second);
        pos.second = position.second - (pos.first - position.first);
        break;
    }
    pos.first = firstTmp;
}
void OperateurRotation::mapPosition(sf::Vector2f &pos, float t) const {
    float angleEnRad = M_PI * 0.5f * t;
    float cosinus = std::cos(angleEnRad);
    float sinus = std::sin(angleEnRad);
    float dx = pos.x - position.first;
    float dy = pos.y - position.second;
    float xTmp;
    switch (sens) {
    case HORAIRE :
        // Rotation 90t° horarire
        xTmp = position.first + cosinus * dx - sinus * dy;
        pos.y = position.second + sinus * dx + cosinus * dy;
        break;
    case ANTIHORAIRE :
        // Rotation 90t° anti-horarire
        xTmp = position.first + cosinus * dx + sinus * dy;
        pos.y = position.second - sinus * dx + cosinus * dy;
        break;
    }
    pos.x = xTmp;
}
void OperateurRotation::mapOperateur(PieceOperateurData &op) const {
    op.accepter(*this);
}

void OperateurRotation::accepter(const OperateurSymetrie &op) {
    PieceOperateurData::accepter(op);
    sens = (sens == HORAIRE) ? ANTIHORAIRE : HORAIRE;
}

//////////////////////////////////////////
//////// CLASSE OperateurSymetrie ////////

OperateurSymetrie::OperateurSymetrie(const std::pair<int,int> &position, Orientation sens)
    : PieceOperateurData{position}
    , sens{sens}
{}

PieceOperateurData* OperateurSymetrie::clone() const {
    return new OperateurSymetrie{*this};
}
const OperateurSymetrie::Orientation OperateurSymetrie::getSens() const {
    return sens;
}

void OperateurSymetrie::mapPosition(std::pair<int, int> &pos) const {
    switch (sens) {
    case VERTICALE :
        // Réflexion par rapport à l'axe des y : (x, y) -> (-x, y)
        pos.first = 2*position.first - pos.first;
        break;
    case HORIZONTALE :
        // Réflexion par rapport à l'axe des x : (x, y) -> (x, -y)
        pos.second = 2*position.second - pos.second;
        break;
    }
}
void OperateurSymetrie::mapPosition(sf::Vector2f &pos, float t) const {
    switch (sens) {
    case VERTICALE :
        pos.x += 2.0f * t * (position.first - pos.x);
        break;
    case HORIZONTALE :
        pos.y += 2.0f * t * (position.second - pos.y);
        break;
    }
}
void OperateurSymetrie::mapOperateur(PieceOperateurData &op) const {
    op.accepter(*this);
}

void OperateurSymetrie::accepter(const OperateurRotation &op) {
    PieceOperateurData::accepter(op);
    sens = (sens == VERTICALE) ? HORIZONTALE : VERTICALE;
}