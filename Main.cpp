#include "GameStateMachine.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	sf::RenderWindow window{
		{ static_cast<unsigned int>(sf::VideoMode::getDesktopMode().width / 3) * 2,
		  static_cast<unsigned int>(sf::VideoMode::getDesktopMode().height / 3) * 2 },
		"Piece Out"
	};
	window.setFramerateLimit(30);
	
	GameStateMachine::initializeStateMachine(window);
	GameStateMachine& context{GameStateMachine::getContext()};

	while (window.isOpen()) {
		context.tick();
    }

    return EXIT_SUCCESS;
}


    //     // if (currentLevel.isInAnimation()) {
    //         // currentLevel.update();
    //         // if (!currentLevel.isOnAnimation() && currentLevel.isFinished()) {
    //         //     message = L"Vous avez gagné !";
    //         // }
    //     // }

    //     // sf::Glyph glyph = symbolsFont.getGlyph(L'\x2B6E', 150, false);
    //     // const sf::Texture& texture = symbolsFont.getTexture(150);
    //     // // Create a sprite to represent the glyph
    //     // sf::Sprite sprite;
    //     // sprite.setTexture(texture);
    //     // sprite.setTextureRect(glyph.textureRect);
    //     // // Set the position of the sprite
    //     // sprite.setPosition(1000.f, 0.f); // Adjust as needed
    //     // fenetre.draw(sprite);
        
    //     // if (glyph.textureRect == sf::IntRect())
    //     // {
    //     //     std::cout << "Glyph not available in the font!" << std::endl;
    //     // }
