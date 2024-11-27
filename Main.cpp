#include <SFML/Graphics.hpp>
#include <iostream>
#include "Niveau.hpp"

int main()
{
	// Parametres graphiques, à replacer au bon endroit
	unsigned int nbPix_x = 1000, nbPix_y = 1000; // TODO
	sf::RenderWindow window{sf::VideoMode{nbPix_x, nbPix_y}, "Piece Out"};
	window.setFramerateLimit(30);

	Piece p1{{{1,1},{2,1},{2,2},{3,2}}, {}, CouleurPiece::VERT};
	PieceOperateur &op = p1.ajouteOpDeplacement({1,1}, OperateurDeplacement::SUD);
	p1.accept(op);
	p1.accept(*(p1.operateurs[0]));
	// PieceOperateur *op = new OperateurDeplacement{p1, {1,2}, EST}; //TODO
	// p1.accept(*op);
	// p1.accept(*op);
	std::vector<Niveau> niveaux{
		{
			8, 6,
			{
				1,1,1,1,1,1,1,1,
				1,0,0,1,1,1,2,1,
				1,0,0,0,1,2,2,1,
				1,0,0,0,0,2,0,1,
				1,1,0,0,0,0,0,1,
				1,1,1,1,1,1,1,1,
			},
			{&p1}
		}
	};

	AfficheurNiveau afficheurNiveau{window, niveaux};
    afficheurNiveau.demarrer();

	// Remarquez que la destruction des objets n'est pas faites
	// et que dans ce code il ne suffit pas de détruire scene_generale et 
	// scene_particuliere car on y a ajouté des objets créés par new et d'autre déclarés dans un bloc... ce qui n'est pas malin.
    return EXIT_SUCCESS;
}
