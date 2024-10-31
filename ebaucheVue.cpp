#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{

	// Parametres graphiques, à replacer au bon endroit
	int MARGIN_LEFT = 100, MARGIN_RIGHT = 100, MARGIN_TOP = 100, MARGIN_BOTTOM = 100;
	int TILE_SIZE = 64;
	int NB_COL = 8, NB_LIGNE = 10;
	unsigned int nbPix_x = MARGIN_LEFT + MARGIN_RIGHT + NB_COL * TILE_SIZE;
	unsigned int nbPix_y = MARGIN_TOP + MARGIN_BOTTOM + NB_LIGNE * TILE_SIZE;

    // définition d'un panneau "central"
    sf::RectangleShape centralPane(sf::Vector2f(NB_COL * TILE_SIZE, NB_LIGNE * TILE_SIZE));
	centralPane.setPosition(MARGIN_LEFT, MARGIN_TOP);
	centralPane.setFillColor(sf::Color::Cyan);

	// plus une trame (lignes horizontales + verticales). En SFML on peut passer par VertexArray
	// Lines est une sf::enum, VertexArray encapsule autant de points que nécessaires pour ces lignes
	sf::VertexArray trame(sf::Lines, (NB_COL+1 + NB_LIGNE+1) * 2); 
	// pour les lignes horizontales
	int n = 0;
	for (int i = 0; i <= NB_LIGNE; ++i)
	{
		trame[n++].position = sf::Vector2f(MARGIN_LEFT, MARGIN_TOP + i*TILE_SIZE);
		trame[n].color = sf::Color::Blue;
		trame[n++].position = sf::Vector2f(nbPix_x - MARGIN_RIGHT, MARGIN_TOP + i*TILE_SIZE);
	}
	// pour les verticales
	for (int i = 0; i <= NB_COL; ++i)
	{
		trame[n++].position = sf::Vector2f(MARGIN_LEFT + i*TILE_SIZE, MARGIN_TOP );
		trame[n].color = sf::Color::Blue;
		trame[n++].position = sf::Vector2f(MARGIN_LEFT + i*TILE_SIZE, nbPix_y - MARGIN_BOTTOM);
	}

	// on peut (ou pas) distinguer la scène générale cadre + frame et la scène particulière (les cases actuelles)
	std::vector<sf::Drawable*> scene_generale;
	std::vector<sf::Drawable*> scene_particuliere;
	scene_generale.push_back(&centralPane);
	scene_generale.push_back(&trame);

	// on place ici la définition d'une texture rouge, probablement qu'il y a mieux à faire (flightweight ? map ?)
   	sf::Texture texture_rouge;
	if (!texture_rouge.loadFromFile("texture.jpg"))
	{
		std::cerr << "Erreur lors du chargement de l'image" << std::endl;
		return EXIT_FAILURE;
	}

	// Opérations graphiques générales
	sf::RenderWindow window{sf::VideoMode{nbPix_x, nbPix_y}, "Piece Out"};
	while (window.isOpen())
	{
		int trig_x = -1, trig_y = -1;
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);
		std::string message = "Mouse Position: ("
							+ std::to_string(int(mouseWorldPos.x)) + ", "
							+ std::to_string(int(mouseWorldPos.y)) + ")";
		if (centralPane.getGlobalBounds().contains(mouseWorldPos))
		{
			sf::Vector2f topLeft = centralPane.getPosition();
			trig_x = (mouseWorldPos.x - topLeft.x) / TILE_SIZE;
			trig_y = (mouseWorldPos.y - topLeft.y) / TILE_SIZE;
			message += " case :" + std::to_string(trig_x) + " ; " + std::to_string(trig_y);
		}
		window.setTitle(message);
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed
				|| (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed 
				&& event.mouseButton.button == sf::Mouse::Left 
				&& centralPane.getGlobalBounds().contains(mouseWorldPos))
			{
				std::cout << "trigger " << trig_x << " " << trig_y << std::endl;
				// création d'un sprite pour afficher une case d'exemple
				sf::Sprite* sprite = new sf::Sprite;
				sprite->setTexture(texture_rouge);
				sf::Vector2u textureSize = texture_rouge.getSize();
				sprite->setScale(static_cast<float>(TILE_SIZE) / textureSize.x, 
								 static_cast<float>(TILE_SIZE) / textureSize.y);
				sprite->setPosition(centralPane.getPosition()
				                    + sf::Vector2f(trig_x * TILE_SIZE, trig_y * TILE_SIZE)); 
				scene_particuliere.push_back(sprite);
			}
        }
        	
		window.clear();
		window.setView(window.getDefaultView());

		// les affichages
		for (sf::Drawable *x : scene_generale) window.draw(*x);
		for (sf::Drawable *x : scene_particuliere) window.draw(*x);
		
        window.display();
    }

	// Remarquez que la destruction des objets n'est pas faites
	// et que dans ce code il ne suffit pas de détruire scene_generale et 
	// scene_particuliere car on y a ajouté des objets créés par new et d'autre déclarés dans un bloc... ce qui n'est pas malin.
    return EXIT_SUCCESS;

}
