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
    etat{ECRAN_NIVEAU},
    police{},
	indiceNiveauActuel{0},
    niveauActuel{niveaux[0], coordOrigine, tailleCase}
{
    chargerResources();
	fenetre.setFramerateLimit(30);
}

//////// METHODES PUBLICS ////////

void AfficheurNiveau::chargerResources()
{
    if (!police.loadFromFile("resources/fonts/Montserrat-Medium.ttf")) {
        std::cerr << "Erreur lors du chargement de la police" << std::endl;
        std::terminate();
    }
}

void AfficheurNiveau::prochainNiveau()
{
    allerAuNiveau(indiceNiveauActuel + 1);
}
void AfficheurNiveau::allerAuNiveau(int indice)
{
    indiceNiveauActuel = indice;
	niveauActuel = Niveau(niveaux[indiceNiveauActuel], coordOrigine, tailleCase);
}

void AfficheurNiveau::demarrer()
{
    fenetre.setTitle("PIECE OUT");
    std::wstring message{L"Piece Out Niveau " + std::to_wstring(indiceNiveauActuel + 1)};

	while (fenetre.isOpen())
	{
		sf::Vector2i posSourisAbsolue{sf::Mouse::getPosition(fenetre)};
		sf::Vector2f posSouris{fenetre.mapPixelToCoords(posSourisAbsolue)};
		        // important, si on redimensionne la fenêtre...

        // la gestion des événements
		sf::Event event;
		while (fenetre.pollEvent(event))
		{
			if (event.type == sf::Event::Closed
				|| (event.type == sf::Event::KeyPressed
                    && event.key.code == sf::Keyboard::Escape))
			{
				fenetre.close();
			}

            switch (etat)
            {
            case Etat::ECRAN_PRINCIPAL:
                break;

            case Etat::ECRAN_SELECTION:
                break;

            case Etat::ECRAN_NIVEAU:
                std::pair<int, int> caseChoisie{-1, -1};
                if (event.type == sf::Event::MouseButtonPressed
                    && event.mouseButton.button == sf::Mouse::Left
                    && niveauActuel.contient(posSouris))
                    {
                        caseChoisie = niveauActuel.mapPixelsEnCases(posSouris);
                        // std::cout << "trigger " << caseChoisie.first << " "
                        //                         << caseChoisie.second << std::endl;
                        int indicePiece = niveauActuel.getDataActuelle(caseChoisie) - 2;
                        if (indicePiece >= 0
                            && niveauActuel.triggerPiece(indicePiece, caseChoisie)
                            && niveauActuel.estGagne())
                        {
                            message = L"Vous avez gagné !";
                        }
                    }
                break;
            }
        }

		fenetre.clear();
		fenetre.setView(fenetre.getDefaultView());

		// les affichages
        switch (etat)
        {
        case Etat::ECRAN_PRINCIPAL:
            break;

        case Etat::ECRAN_SELECTION:
            break;

        case Etat::ECRAN_NIVEAU:
            fenetre.draw(niveauActuel);
            // TODO
            sf::Text text{message, police};
            text.setFillColor(sf::Color::White);
            text.setCharacterSize(100); // en pixels
            fenetre.draw(text);

            break;
        }

        fenetre.display();
    }
}
