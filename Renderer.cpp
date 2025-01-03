#include "Renderer.hpp"
#include "LevelEventHandler.hpp"
#include "ResourceManager.hpp"

Renderer::Renderer(const std::vector<LevelData>& levels)
    : levels{levels}
    , window{ { static_cast<unsigned int>(sf::VideoMode::getDesktopMode().width / 3) * 2,
			    static_cast<unsigned int>(sf::VideoMode::getDesktopMode().height / 3) * 2 },
			  "Piece Out" }
    , centerCoords{window.mapPixelToCoords(sf::Vector2i{window.getSize()}) / 2.0f}
	, gridSizeInPixels{std::min(centerCoords.x, centerCoords.y) / 6.0f}
	, indexCurrentLevel{0}
    , currentLevel{levels[0], centerCoords, gridSizeInPixels}
{
	window.setFramerateLimit(30);
}

void Renderer::run()
{
    std::wstring message{L"Piece Out Niveau " + std::to_wstring(indexCurrentLevel + 1)};

    // LevelEventHandler handler{currentLevel};
    LevelStateContext context{currentLevel};

	while (window.isOpen())
	{
		sf::Vector2i mousePos{sf::Mouse::getPosition(window)};
        context.handleMousePositionUpdate(window.mapPixelToCoords(mousePos));

        // la gestion des événements
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed
				|| (event.type == sf::Event::KeyPressed
                    && event.key.code == sf::Keyboard::Escape))
			{
				window.close();
			}

            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left)
            {
                context.handleMouseClick();
            }

            if (event.type == sf::Event::MouseButtonReleased
                && event.mouseButton.button == sf::Mouse::Left)
            {
                context.handleMouseRelease();
            }
        }

        window.clear();
        window.setView(window.getDefaultView());

        // les affichages
        if (currentLevel.isInAnimation()) {
            // currentLevel.update();
            // if (!currentLevel.isOnAnimation() && currentLevel.isFinished()) {
            //     message = L"Vous avez gagné !";
            // }
        }
                    
        window.draw(currentLevel);
        // TODO
        sf::Text text{message, ResourceManager::getInstance().getTextFont()};
        text.setFillColor(sf::Color::Yellow);
        text.setCharacterSize(100); // en pixels
        window.draw(text);

        // sf::RectangleShape rect{sf::Vector2f{150,150}};
        // rect.setPosition(1000, 0);
        // rect.setFillColor(sf::Color::Blue);
        // fenetre.draw(rect);

        // sf::Glyph glyph = symbolsFont.getGlyph(L'\x2B6E', 150, false);
        // const sf::Texture& texture = symbolsFont.getTexture(150);
        // // Create a sprite to represent the glyph
        // sf::Sprite sprite;
        // sprite.setTexture(texture);
        // sprite.setTextureRect(glyph.textureRect);
        // // Set the position of the sprite
        // sprite.setPosition(1000.f, 0.f); // Adjust as needed
        // fenetre.draw(sprite);
        
        // if (glyph.textureRect == sf::IntRect())
        // {
        //     std::cout << "Glyph not available in the font!" << std::endl;
        // }

        window.display();
    
    }
}