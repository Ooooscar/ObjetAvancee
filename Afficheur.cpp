#include "Afficheur.hpp"

////////////////////////////////////////
//////// CLASSE AfficheurNiveau ////////

//////// CONSTRUCTEURS ////////

AfficheurNiveau::AfficheurNiveau(const std::vector<NiveauData> &niveaux) :
	// ici les deep-copies sont faites pour les `std::vector`
	// TODO : éviter la copie de `std::vector<NiveauData>& niveaux` ici...
	niveaux{niveaux},
	fenetre{ { static_cast<unsigned int>(sf::VideoMode::getDesktopMode().width / 3) * 2,
			   static_cast<unsigned int>(sf::VideoMode::getDesktopMode().height / 3) * 2 },
			 "Piece Out" },
	coordOrigine{fenetre.mapPixelToCoords(sf::Vector2i{fenetre.getSize()}) / 2.0f},
	tailleCase{std::min(coordOrigine.x, coordOrigine.y) / 6.0f},
	indiceNiveauActuel{0}, niveauActuel{niveaux[0], coordOrigine, tailleCase}
{
	fenetre.setFramerateLimit(30);
}

//////// METHODES PUBLICS ////////

void AfficheurNiveau::prochainNiveau() {
    allerAuNiveau(indiceNiveauActuel + 1);
}
void AfficheurNiveau::allerAuNiveau(int indice) {
    indiceNiveauActuel = indice;
	niveauActuel = Niveau(niveaux[indiceNiveauActuel], coordOrigine, tailleCase);
}

void AfficheurNiveau::demarrer()
{
	// Opérations graphiques générales
	while (fenetre.isOpen())
	{
		std::pair<int, int> caseChoisie{-1, -1};
		sf::Vector2i posSourisAbsolue{sf::Mouse::getPosition(fenetre)};
		// important, si on redimensionne la fenêtre...
		sf::Vector2f posSouris{fenetre.mapPixelToCoords(posSourisAbsolue)};
		std::string message = "Mouse Position: ("
							+ std::to_string(int(posSouris.x)) + ", "
							+ std::to_string(int(posSouris.y)) + ")";
		bool sourisSurNiveau{niveauActuel.contient(posSouris)};
		if (sourisSurNiveau)
		{
			caseChoisie = niveauActuel.mapPixelsEnCases(posSouris);
			message += " case :" + std::to_string(caseChoisie.first) + " ; " + std::to_string(caseChoisie.second);
		}
		fenetre.setTitle(message);
		
		sf::Event event;
		while (fenetre.pollEvent(event))
		{
			if (event.type == sf::Event::Closed
				|| (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				fenetre.close();
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left 
				&& sourisSurNiveau)
			{
				// std::cout << "trigger " << caseChoisie.first << " " << caseChoisie.second << std::endl;
				int indicePiece = niveauActuel.getDataActuelle(caseChoisie) - 2;
				if (indicePiece >= 0
					&& niveauActuel.triggerPiece(indicePiece, caseChoisie)
					&& niveauActuel.estGagne())
				{
					std::cout << "YOU WIN !!!" << std::endl;
				}
			}
        }
        	
		fenetre.clear();
		fenetre.setView(fenetre.getDefaultView());

		// les affichages
		fenetre.draw(niveauActuel);
		
        fenetre.display();
    }
}
