#include "Afficheur.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	std::vector<NiveauData> niveaux{
		{
			9, 8,
			{
				1,1,1,1,1,1,1,1,1,
				1,1,0,0,1,1,0,0,1,
				1,0,0,0,0,0,0,0,1,
				1,1,1,0,0,0,0,0,1,
				1,1,1,2,0,0,3,3,1,
				1,1,2,2,2,0,0,3,1,
				1,1,1,1,1,1,0,3,1,
				1,1,1,1,1,1,1,1,1,
			}
		},
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

	niveaux[0].ajouterPiece(
			{{1,2},{2,1},{2,2},{3,2}},
			CouleurPiece::VERT,
			{
				new OperateurDeplacement{{1,2}, OperateurDeplacement::OUEST},
				new OperateurDeplacement{{3,2}, OperateurDeplacement::EST},
				new OperateurRotation{{2,2}, OperateurRotation::ANTIHORAIRE}
			}
		);
	niveaux[0].ajouterPiece(
			{{6,1},{7,1},{7,2},{7,3}},
			CouleurPiece::BLEU,
			{
				new OperateurDeplacement{{7,1}, OperateurDeplacement::NORD},
				new OperateurDeplacement{{7,3}, OperateurDeplacement::SUD},
				new OperateurSymetrie{{7,2}, OperateurSymetrie::HORIZONTALE}
			}
		);

	// PieceData &p10 = niveaux[1].ajouterPiece({{2,1},{2,2},{2,3},{3,2}}, CouleurPiece::VERT);
	// p10.ajouterOpDeplacement({2,1}, OperateurDeplacement::NORD);
	// p10.ajouterOpDeplacement({2,3}, OperateurDeplacement::SUD);
	// p10.ajouterOpRotation({2,2}, OperateurRotation::ANTIHORAIRE);
	// std::cout << op.position.first << ", " << op.position.second << std::endl;

	AfficheurNiveau afficheurNiveau{niveaux};
    afficheurNiveau.demarrer();

    return EXIT_SUCCESS;
}
