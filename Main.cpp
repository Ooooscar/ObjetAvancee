#include "Niveau.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	std::vector<NiveauData> niveaux{
		{
			9, 7,
			{
				1,1,1,1,1,1,1,1,1,
				1,1,0,1,1,1,2,1,1,
				1,0,0,0,1,2,2,0,1,
				1,0,0,1,1,1,2,0,1,
				1,0,0,0,0,0,0,0,1,
				1,1,0,0,0,0,0,1,1,
				1,1,1,1,1,1,1,1,1,
			}
		}
	};
	PieceData &p1 = niveaux[0].ajouterPiece({{2,1},{2,2},{2,3},{3,2}}, CouleurPiece::VERT);
	p1.ajouterOpDeplacement({2,1}, OperateurDeplacement::NORD);
	p1.ajouterOpDeplacement({2,3}, OperateurDeplacement::SUD);
	p1.ajouterOpRotation({2,2}, OperateurRotation::ANTIHORAIRE);

	AfficheurNiveau afficheurNiveau{niveaux};
    afficheurNiveau.demarrer();

    return EXIT_SUCCESS;
}
