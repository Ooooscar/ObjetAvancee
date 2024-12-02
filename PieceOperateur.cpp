#include "PieceOperateur.hpp"

///////////////////////////////////////
//////// CLASSE PieceOperateur ////////

PieceOperateur::PieceOperateur(const std::pair<int,int> &position) :
    position{std::move(position)}
{}

void PieceOperateur::accepter(const OperateurDeplacement &op) {
    op.mapPosition(position);
}
void PieceOperateur::accepter(const OperateurRotation &op) {
    op.mapPosition(position);
};
void PieceOperateur::accepter(const OperateurSymetrie &op) {
    op.mapPosition(position);
};

/////////////////////////////////////////////
//////// CLASSE OperateurDeplacement ////////

OperateurDeplacement::OperateurDeplacement(const std::pair<int,int> &position, Orientation sens) :
    PieceOperateur{position},
    sens{sens}
{}

void OperateurDeplacement::mapPosition(std::pair<int, int> &pos) const {
    switch (sens) {
    case NORD : pos.second--; break;
    case SUD : pos.second++; break;
    case EST : pos.first++; break;
    case OUEST : pos.first--; break;
    }
}
void OperateurDeplacement::mapOperateur(PieceOperateur &op) const {
    op.accepter(*this);
}

void OperateurDeplacement::accepter(const OperateurRotation &op) {
    PieceOperateur::accepter(op);
    switch (op.sens) {
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
    PieceOperateur::accepter(op);
    switch (op.sens) {
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

OperateurRotation::OperateurRotation(const std::pair<int,int> &position, Orientation sens) :
    PieceOperateur{position},
    sens{sens}
{}

void OperateurRotation::mapPosition(std::pair<int, int> &pos) const {
    switch (sens) {
    case HORAIRE :
        // Rotation 90° horarire : (x, y) -> (y, -x)
        pos = std::move(std::make_pair<int>(
                position.first - (pos.second - position.second),
                position.second + (pos.first - position.first)
            ));
        break;
    case ANTIHORAIRE :
        // Rotation 90° anti-horarire : (x, y) -> (-y, x)
        pos = std::move(std::make_pair<int>(
                position.first + (pos.second - position.second),
                position.second - (pos.first - position.first)
            ));
        break;
    }
}
void OperateurRotation::mapOperateur(PieceOperateur &op) const {
    op.accepter(*this);
}

void OperateurRotation::accepter(const OperateurSymetrie &op) {
    PieceOperateur::accepter(op);
    sens = (sens == HORAIRE) ? ANTIHORAIRE : HORAIRE;
}

//////////////////////////////////////////
//////// CLASSE OperateurSymetrie ////////

OperateurSymetrie::OperateurSymetrie(const std::pair<int,int> &position, Orientation sens) :
    PieceOperateur{position},
    sens{sens}
{}

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
void OperateurSymetrie::mapOperateur(PieceOperateur &op) const {
    op.accepter(*this);
}

void OperateurSymetrie::accepter(const OperateurRotation &op) {
    PieceOperateur::accepter(op);
    sens = (sens == VERTICALE) ? HORIZONTALE : VERTICALE;
}
