#ifndef _PIECEOUT_RENDERER
#define _PIECEOUT_RENDERER
#include "Level.hpp"
#include <SFML/Graphics.hpp>

class Renderer
{
private:
    const std::vector<LevelData> levels;
    sf::RenderWindow window;
    sf::Vector2f centerCoords;
    float gridSizeInPixels;

    int indexCurrentLevel;
    Level currentLevel;
public:
    Renderer(const std::vector<LevelData>& levels);
    void run();
};

#endif