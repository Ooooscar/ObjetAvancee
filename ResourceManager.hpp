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
public:
    static ResourceManager& getInstance();
    sf::Font& getTextFont();
    sf::Font& getSymbolsFont();
};

#endif