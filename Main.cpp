#include <SFML/Graphics.hpp>
#include <iostream>
#include "Niveau.hpp"

int main()
{
	// Parametres graphiques, à replacer au bon endroit
	unsigned int nbPix_x = 1000, nbPix_y = 1000; // TODO
	sf::RenderWindow window{sf::VideoMode{nbPix_x, nbPix_y}, "Piece Out"};
	window.setFramerateLimit(30);

	std::vector<NiveauData> niveaux{
		{
			8, 6,
			{
				1,1,1,1,1,1,1,1,
				1,0,0,1,1,1,2,1,
				1,0,0,0,1,2,2,1,
				1,0,0,0,0,2,0,1,
				1,1,0,0,0,0,0,1,
				1,1,1,1,1,1,1,1,
			}
		}
	};

	PieceData &p1 = niveaux[0].ajoutePiece({{1,1},{2,1},{2,2},{3,2}}, CouleurPiece::VERT);
	p1.ajouteOpDeplacement({1,1}, OperateurDeplacement::SUD);
	p1.ajouteOpDeplacement({3,2}, OperateurDeplacement::EST);
	p1.ajouteOpRotation({2,1}, OperateurRotation::ANTIHORAIRE);

	AfficheurNiveau afficheurNiveau{window, niveaux};
    afficheurNiveau.demarrer();

    return EXIT_SUCCESS;
}
