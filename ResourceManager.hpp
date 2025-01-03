#ifndef _PIECEOUT_RESOURCE_MANAGER
#define _PIECEOUT_RESOURCE_MANAGER
#include <SFML/Graphics.hpp>

class ResourceManager
{
private:
    static ResourceManager instance;
    ResourceManager() = default;
    virtual ~ResourceManager();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

	sf::Font* textFont;
	sf::Font* symbolsFont;

    void loadTextFont();
    void loadSymbolsFont();
public:
    static ResourceManager& getInstance();
    const sf::Font& getTextFont();
    const sf::Font& getSymbolsFont();
};

#endif